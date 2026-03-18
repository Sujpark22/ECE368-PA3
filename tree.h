#ifndef TREE_H
#define TREE_H
#include <stdio.h>

typedef struct Node {
    int num, width, height; // node number, width, and height of the rectangle
    
    char type; // H, V, or N
    
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

// build tree from postorder file
Node *Build_Tree_From_File(FILE *fptr);

// computes dimensions of each node and stores in node struct
// combined with pack.c from pa2
void Compute_Dimensions(Node *root);

// free tree memory
void Free_Tree(Node *root);

#endif