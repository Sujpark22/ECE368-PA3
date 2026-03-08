#ifndef FILES_H
#define FILES_H

#include "tree.h"

void Write_Postorder(Node *root, FILE *fptr); // output file 1
void Write_Dimensions_Postorder(Node *root, FILE *fptr); // output file 2
void Write_Coords(Node *root, FILE *fptr); // output file 3

#endif