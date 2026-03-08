#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "pack.h"
#include "files.h"

int main(int argc, char *argv[])
{
    if (argc != 5) {
        return EXIT_FAILURE;
    }

    // file pointers for input and output files
    FILE *in_file_ptr = NULL;
    FILE *out_file1_ptr = NULL;
    FILE *out_file2_ptr = NULL;
    FILE *out_file3_ptr = NULL;

    // Open input and output files
    // close files if any file fails to open and return EXIT_FAILURE
    in_file_ptr = fopen(argv[1], "r");
    if (in_file_ptr == NULL) {
        return EXIT_FAILURE;
    }

    out_file1_ptr = fopen(argv[2], "w");
    if (out_file1_ptr == NULL) {
        fclose(in_file_ptr);
        return EXIT_FAILURE;
    }

    out_file2_ptr = fopen(argv[3], "w");
    if (out_file2_ptr == NULL) {
        fclose(in_file_ptr);
        fclose(out_file1_ptr);
        return EXIT_FAILURE;
    }

    out_file3_ptr = fopen(argv[4], "w");
    if (out_file3_ptr == NULL) {
        fclose(in_file_ptr);
        fclose(out_file1_ptr);
        fclose(out_file2_ptr);
        return EXIT_FAILURE;
    }

    // build tree from input file
    Node *tree = Build_Tree_From_File(in_file_ptr);
    fclose(in_file_ptr);

    if (tree == NULL) {
        fclose(out_file1_ptr);
        fclose(out_file2_ptr);
        fclose(out_file3_ptr);
        return EXIT_FAILURE;
    }

    // Write the postorder traversal of the tree to out_file1
    Write_Postorder(tree, out_file1_ptr);
    fclose(out_file1_ptr);

    // Compute the dimensions of each node in the tree and store in node struct
    Compute_Dimensions(tree);
    // Write the dimensions of each node in postorder traversal to out_file2
    Write_Dimensions_Postorder(tree, out_file2_ptr);
    fclose(out_file2_ptr);

    // Compute the coordinates of each node in the tree and store in node struct
    Compute_Coords(tree, 0, 0);
    // Write the coordinates of each node in postorder traversal to out_file3
    Write_Coords(tree, out_file3_ptr);
    fclose(out_file3_ptr);
    
    // Free the memory allocated for the tree
    Free_Tree(tree);

    return EXIT_SUCCESS;
}