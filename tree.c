#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

typedef struct S { 
    Node *data; // actual value stored
    struct S *next; // pointer to next stack element
} Stack;

static void push(Stack **top, Node *n) { 
    Stack *s = malloc(sizeof(Stack)); 
    s->data=n; 
    s->next=*top; 
    *top=s; 
}

static Node *pop(Stack **top) { 
    if(*top == NULL)
        return NULL; 
    Stack *s=*top; 
    Node *n=s->data; 
    *top=s->next; 
    free(s); 
    return n; 
}

Node *Build_Tree_From_File(FILE *fptr) {
    Stack *s = NULL;
    char line[256];

    // Read lines until EOF, building tree bottom-up using stack
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0]=='\n' || line[0]=='\r') 
            continue;

        // Allocate new node
        Node *node = calloc(1, sizeof(Node));
        if (node == NULL) { 
            while(s != NULL)
                Free_Tree(pop(&s));  
            return NULL; 
        }

        // if line starts with 'H' or 'V', its an internal node; otherwise it's a leaf
        if (line[0]=='H' || line[0]=='V') {
            node->type = line[0];
            node->right = pop(&s);
            node->left  = pop(&s);
            if (!node->left || !node->right) {
                free(node);
                while(s != NULL)
                    Free_Tree(pop(&s));
                return NULL;
            }
            node->left->parent  = node;
            node->right->parent = node;
        } 
        else {
            node->type = 'N';
            if (sscanf(line, "%d(%d,%d)", &node->num, &node->width, &node->height) != 3) {
                free(node);
                while(s != NULL)
                    Free_Tree(pop(&s));
                return NULL;
            }
        }
        push(&s, node);
    }

    // After processing all lines, stack should have the root left
    Node *root = pop(&s);
    if (s != NULL) { 
        Free_Tree(root); 
        while(s != NULL)
            Free_Tree(pop(&s)); 
        return NULL; 
    }
    return root;
}

void Compute_Dimensions(Node *root)
{
    if (root == NULL)
        return;

    Compute_Dimensions(root->left);
    Compute_Dimensions(root->right);

    if (root->type == 'H') // compute dimensions for H node
    {
        // set width to max of left and right subtree widths
        if (root->left->width > root->right->width)
        {
            root->width = root->left->width;
        }
        else
        {
            root->width = root->right->width;
        }

        // set height to sum of left and right subtree heights
        root->height = root->left->height + root->right->height;
    }
    else if (root->type == 'V') // compute dimensions for V node
    {
        // set width to sum of left and right subtree widths
        root->width = root->left->width + root->right->width;

        // set height to max of left and right subtree heights
        if (root->left->height > root->right->height)
        {
            root->height = root->left->height;
        }
        else
        {
            root->height = root->right->height;
        }
    }
}

// free tree memory using postorder traversal
void Free_Tree(Node *root) {
    if (root == NULL) 
        return;
    Free_Tree(root->left);
    Free_Tree(root->right);
    free(root);
}