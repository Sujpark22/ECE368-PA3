#ifndef REROOTING_H
#define REROOTING_H
#include <stdio.h>
#include "tree.h"

// preorder of tree rerooted at end of LR path
void Write_LR(Node *root, FILE *fptr); // output file 1

// preorder of tree rerooted at end of RL path
void Write_RL(Node *root, FILE *fptr); // output file 2

// preorder of original tree with dimensions (width,height) for each node
void Write_Rdim(Node *root, FILE *fptr); // output file 3

// preorder of tree rerooted at edge that minimizes area
void Write_Optimal(Node *root, FILE *fptr); // output file 4

#endif