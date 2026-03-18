#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "reroot.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        return EXIT_FAILURE;
    }

    FILE *in_file_ptr = fopen(argv[1], "r");  
    if (in_file_ptr == NULL) { 
        return EXIT_FAILURE;
    }

    FILE *out_file1_ptr = fopen(argv[2], "w");  
    if (out_file1_ptr == NULL)  { 
        fclose(in_file_ptr); 
        return EXIT_FAILURE; 
    }

    FILE *out_file2_ptr  = fopen(argv[3], "w");  
    if (out_file2_ptr == NULL)  { 
        fclose(in_file_ptr); 
        fclose(out_file1_ptr); 
        return EXIT_FAILURE; 
    }

    FILE *out_file3_ptr = fopen(argv[4], "w"); 
    if (out_file3_ptr == NULL)  { 
        fclose(in_file_ptr); 
        fclose(out_file1_ptr); 
        fclose(out_file2_ptr); 
        return EXIT_FAILURE; 
    }

    FILE *out_file4_ptr = fopen(argv[5], "w");  
    if (out_file4_ptr == NULL)  { 
        fclose(in_file_ptr); 
        fclose(out_file1_ptr); 
        fclose(out_file2_ptr); 
        fclose(out_file3_ptr); 
        return EXIT_FAILURE; 
    }

    Node *tree = Build_Tree_From_File(in_file_ptr);
    fclose(in_file_ptr);

    if (tree == NULL) { 
        fclose(out_file1_ptr); 
        fclose(out_file2_ptr); 
        fclose(out_file3_ptr); 
        fclose(out_file4_ptr); 
        return EXIT_FAILURE; 
    }

    Compute_Dimensions(tree);

    Write_LR(tree, out_file1_ptr);   
    fclose(out_file1_ptr);

    Write_RL(tree, out_file2_ptr);   
    fclose(out_file2_ptr);

    Write_Rdim(tree, out_file3_ptr);          
    fclose(out_file3_ptr);

    Write_Optimal(tree, out_file4_ptr);       
    fclose(out_file4_ptr);

    // Free the memory allocated for the tree
    Free_Tree(tree);

    return EXIT_SUCCESS;
}