#ifndef TREE_H
#define TREE_H
#include <stdio.h>

typedef struct Node {
    int num, width, height; // node number, width, and height of the rectangle

    int x, y; // coordinates of the bottom-left corner of the rectangle

    char type; // H, V, or N
    struct Node *left;
    struct Node *right;
} Node;

Node *Build_Tree_From_File(FILE *fptr);
void Free_Tree(Node *root);

#endif