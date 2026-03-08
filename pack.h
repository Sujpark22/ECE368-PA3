#ifndef PACK_H
#define PACK_H

#include "tree.h"

void Compute_Dimensions(Node *root); // computes dimensions of each node and stores in node struct
void Compute_Coords(Node *root, int x, int y); // computes coordinates of each node and stores in node struct

#endif