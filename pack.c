#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

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

void Compute_Coords(Node *root, int x, int y)
{
    if (root == NULL)
        return;

    root->x = x;
    root->y = y;

    if (root->type == 'H')
    {
        // right subtree below
        Compute_Coords(root->right, x, y);

        // left subtree above
        Compute_Coords(root->left, x, y + root->right->height);
    }
    else if (root->type == 'V')
    {
        // left subtree left
        Compute_Coords(root->left, x, y);

        // right subtree right
        Compute_Coords(root->right, x + root->left->width, y);
    }
}