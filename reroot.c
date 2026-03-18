
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "reroot.h"

// combine two packings using type
static void combine(char type, int w1, int h1, int w2, int h2, int *width, int *height) {
    if (type == 'H') {
        *width = w1 > w2 ? w1 : w2;
        *height = h1 + h2;
    } 
    else {
        *width = w1 + w2;
        *height = h1 > h2 ? h1 : h2;
    }
}

// preorder print for leaves, H/V for internal
static void preorder_print(Node *node, FILE *fptr) {
    if (node == NULL) 
        return;
    if (node->type == 'N')
        fprintf(fptr, "%d(%d,%d)\n", node->num, node->width, node->height);
    else
        fprintf(fptr, "%c\n", node->type);
    preorder_print(node->left,  fptr);
    preorder_print(node->right, fptr);
}

// struct to store info needed to undo a rerooting at an edge
typedef struct {
    Node *child; // node whose edge was rerooted
    Node *parent; // child's parent
    Node *gParent; // Parent's parent
    int C_left; // 1 if child == parent->left before the call
    Node *movedC; // the child of parent that got moved during reroot
    int P_left; // 1 if parent == grandparent->left before the call
} RerootInfo;

// Reroot at edge between child and parent, updating info struct for undoing later
static void reroot_edge(Node *C, RerootInfo *info) {
    Node *Parent = C->parent;
    Node *GParent = Parent->parent;

    info->child = C;
    info->parent = Parent;
    info->gParent = GParent;
    info->C_left = (C == Parent->left);
    info->P_left = (Parent == GParent->left);

    if (info->C_left) {
        // reroot at Parent->left edge
        info->movedC = Parent->right;
        Parent->right = GParent;
        if (info->P_left) 
            GParent->left  = info->movedC;
        else
            GParent->right = info->movedC;
        if (info->movedC) 
            info->movedC->parent = Parent;
    } else {
        // reroot at Parent->right edge
        info->movedC = Parent->left;
        Parent->left = GParent;
        if (info->P_left) 
            GParent->left  = info->movedC;
        else
            GParent->right = info->movedC;
        if (info->movedC) 
            info->movedC->parent = Parent;
    }
    // finalize new parent-child relationships
    GParent->parent = Parent;
    Parent->parent = NULL;
    // recompute dimensions for affected nodes
    Compute_Dimensions(GParent);
    Compute_Dimensions(Parent);
}

// Undo a rerooting at edge between child and parent using info from the call to reroot_edge
static void undo_reroot_edge(const RerootInfo *info) {
    Node *Parent = info->parent;
    Node *GParent = info->gParent;

    if (info->C_left) {
        Parent->right = info->movedC;
        if (info->P_left) 
            GParent->left = Parent;
        else
            GParent->right = Parent;
        if (info->movedC) 
            info->movedC->parent = Parent;
    } else {
        Parent->left = info->movedC;
        if (info->P_left) 
            GParent->left = Parent;
        else
            GParent->right = Parent;
        if (info->movedC) 
            info->movedC->parent = Parent;
    }
    Parent->parent = GParent;
    GParent->parent = NULL;
    Compute_Dimensions(Parent);
    Compute_Dimensions(GParent);
}


// helper to collect path from root down to leaf following LR or RL pattern
static int collect_lr_path(Node *root, Node **path, int max, int go_left_first) {
    int len = 0;
    Node *curr = root;
    int go_left = go_left_first;
    while (curr && len < max) {
        path[len++] = curr;
        if (curr->type == 'N')
            break;
        if (go_left)
            curr = curr->left;
        else
            curr = curr->right;
        go_left = !go_left;
    }
    return len;
}

// helper to collect path from root down to leaf following parent pointers
static int collect_root_path(Node *node, Node **path)
{
    int len = 0;
    Node *curr = node;

    // walk up to root
    while (curr != NULL)
    {
        path[len++] = curr;
        curr = curr->parent;
    }

    // reverse so root is at index 0
    int low = 0;
    int high = len - 1;
    while (low < high)
    {
        Node *tmp = path[low];
        path[low]  = path[high];
        path[high]  = tmp;
        low++;
        high--;
    }
    return len;
}

// helper to apply a sequence of reroots along a path, returning the new root after all reroots
static Node *apply_reroots(Node **path, int start, int end, RerootInfo *infos) {
    Node *new_root = NULL;
    for (int i = start; i <= end; i++) {
        reroot_edge(path[i], &infos[i]);
        new_root = path[i]->parent;   // after rerooting at path[i], it becomes the parent of its old parent
    }
    return new_root;
}

// helper to undo a sequence of reroots in reverse order
static void undo_reroots(const RerootInfo *infos, int start, int end) {
    for (int i = end; i >= start; i--)
        undo_reroot_edge(&infos[i]);
}

// LR and RL rerooting
static void write_lr_rl(Node *root, FILE *fptr, int go_left_first) {
    if (root == NULL) 
        return;
    Node *path[4096];
    int len = collect_lr_path(root, path, 4096, go_left_first);
    if (len < 3) { 
        preorder_print(root, fptr); 
        return; 
    }
    RerootInfo infos[4096];
    Node *new_root = apply_reroots(path, 2, len-1, infos);
    Compute_Dimensions(new_root);
    preorder_print(new_root, fptr);
    undo_reroots(infos, 2, len-1);
    Compute_Dimensions(root);
}

void Write_LR(Node *root, FILE *fptr) { 
    write_lr_rl(root, fptr, 1); 
}
void Write_RL(Node *root, FILE *fptr) { 
    write_lr_rl(root, fptr, 0); 
}



// output files 3 and 4

// state struct for tracking best rerooting while traversing for output 3 and 4
typedef struct {
    FILE *fptr; // NULL = skip printing
    long long best_area; // initialized to original area
    int curr_idx; // index of current rerootable node in preorder
    int best_idx; // index of best rerooted representation found
    Node *best_node; // target node that gives best area when rerooted at its edge with parent
} State;

// helper to traverse tree and compute dimensions for output 3 and track best rerooting for output 4
static void traverse(Node *n, char ptype, int comp_w, int comp_h, int depth, State *st) {
    if (n == NULL) 
        return;

    int re_rootable = (depth >= 2);

    if (re_rootable) {
        int rw, rh;
        combine(ptype, n->width, n->height, comp_w, comp_h, &rw, &rh);
        long long area = (long long)rw * rh;
        st->curr_idx++;
        if (area < st->best_area) {
            st->best_area = area;
            st->best_idx  = st->curr_idx;
            st->best_node = n;
        }
        if (st->fptr) {
            if (n->type == 'N')
                fprintf(st->fptr, "%d(%d,%d)\n", n->num, rw, rh);
            else
                fprintf(st->fptr, "%c(%d,%d)\n", n->type, rw, rh);
        }
    } else {
        if (st->fptr) {
            if (n->type == 'N')
                fprintf(st->fptr, "%d\n", n->num);
            else
                fprintf(st->fptr, "%c\n", n->type);
        }
    }

    if (n->type == 'N') 
        return;

    int lw, lh, rw2, rh2;
    combine(ptype, comp_w, comp_h, n->right->width,  n->right->height, &lw, &lh);
    combine(ptype, comp_w, comp_h, n->left->width,   n->left->height,  &rw2, &rh2);

    traverse(n->left,  n->type, lw,  lh,  depth+1, st);
    traverse(n->right, n->type, rw2, rh2, depth+1, st);
}

// preorder of original tree with dimensions (width,height) for each node
void Write_Rdim(Node *root, FILE *fptr) {
    if (root == NULL) 
        return;
    long long A0 = (long long)root->width * root->height;
    State st = { fptr, A0, 0, 0, NULL };
    traverse(root, root->type, 0, 0, 0, &st);
}

// tracks best rerooting while traversing
void Write_Optimal(Node *root, FILE *fptr) {
    if (root == NULL) 
        return;

    long long A0 = (long long)root->width * root->height;
    State st = { NULL, A0, 0, 0, NULL };
    traverse(root, root->type, 0, 0, 0, &st);

    // print original tree if no rerooting improves area
    if (st.best_node == NULL || st.best_area >= A0) {
        preorder_print(root, fptr);
        return;
    }

    // reroot at best node and print
    Node *path[4096];
    RerootInfo infos[4096];
    int len = collect_root_path(st.best_node, path);
    Node *new_root = apply_reroots(path, 2, len-1, infos);
    Compute_Dimensions(new_root);
    preorder_print(new_root, fptr);
    undo_reroots(infos, 2, len-1);
    Compute_Dimensions(root);
}