#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

// output file 1
void Write_Postorder(Node *root, FILE *fptr)
{
    if (root == NULL)
        return;

    // postorder: left, right, root
    Write_Postorder(root->left, fptr);
    Write_Postorder(root->right, fptr);

    // if H or V, print type
    // if N, print num(width,height)
    if (root->type == 'N')
        fprintf(fptr, "%d(%d,%d)\n", root->num, root->width, root->height);
    else
        fprintf(fptr, "%c\n", root->type);
}

// output file 2
void Write_Dimensions_Postorder(Node *root, FILE *fptr)
{
    if (root == NULL)
        return;

    // postorder: left, right, root
    Write_Dimensions_Postorder(root->left, fptr);
    Write_Dimensions_Postorder(root->right, fptr);

    // if H or V, print type(width,height)
    // if N, print num(width,height)
    if (root->type == 'N')
        fprintf(fptr, "%d(%d,%d)\n", root->num, root->width, root->height);
    else
        fprintf(fptr, "%c(%d,%d)\n", root->type, root->width, root->height);
}

// output file 3
void Write_Coords(Node *root, FILE *fptr)
{
    if (root == NULL)
        return;

    // postorder: left, right, root
    if (root->type == 'N')
    {
        // print num((width,height)(x,y)) for H, V, and N node
        fprintf(fptr, "%d((%d,%d)(%d,%d))\n", root->num, root->width, root->height, root->x, root->y);
    }
    //
    Write_Coords(root->left, fptr);
    Write_Coords(root->right, fptr);
}