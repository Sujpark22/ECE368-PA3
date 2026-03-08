#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

Node *Build_Tree_From_File(FILE *fptr)
{
    // create a buffer to read each line of file
    char line[256];
    // return null if file is empty or there is error
    if (fgets(line, sizeof(line), fptr) == NULL)
        return NULL;

    Node *node = malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    
    // initialize node members to default values
    node->left = NULL;
    node->right = NULL;
    node->x = 0;
    node->y = 0;

    // if line starts with H or V, set node type and build left and right subtrees
    if (line[0] == 'H' || line[0] == 'V')
    {
        node->type = line[0];

        node->left = Build_Tree_From_File(fptr);
        node->right = Build_Tree_From_File(fptr);

        if (node->left == NULL || node->right == NULL)
        {
            Free_Tree(node);
            return NULL;
        }
    }
    else // if not a H or V node
    {
        node->type = 'N'; // will have format [num(width,height)] - set as leaf node
        // parse the line to extract node number, width, and height
        if (sscanf(line, "%d(%d,%d)", &node->num, &node->width, &node->height) != 3)
        {
            // free node and return null if line does not get scanned carrectly
            free(node);
            return NULL;
        }
    }

    return node;
}

void Free_Tree(Node *root)
{
    if (root == NULL)
        return;

    Free_Tree(root->left);
    Free_Tree(root->right);
    free(root);
}