/*
    simple solver for sudoku
    just do analysis for each lattice, and try
*/

#include <stdio.h>

#include "sudoku.h"

int main(int argc, char **argv)
{
    char *filename="input.txt";

    printf("input: %s\n\n", filename);

    matrix mat;
    mat.ntry=0;
    mat.nstep=0;

    parse_args(argc, argv, &mat);

    mat_init_unset(&mat);

    readtxt(filename, &mat);

    unset=mat.unset;
    printf("unset lattices: %i\n", unset);
    printf("initial state:\n");
    print_mat(&mat);
    printf("\n");

    nsol=0;
    ntry=0;
    lchn=-1;

    int found=mat_fill_notry(&mat);

    printf("\nafter fill without try:\n");
    if(found==SCAN_ERROR) {
        printf("    error for matrix\n");
        return -1;
    }

    if(found==SCAN_SUCC) {
        printf("    finished without try\n");
    } else {
        printf("    matrix after fill without try\n");
        printf("    unset lattices: %i\n", mat.unset);
        print_mat(&mat);
    }
    printf("\n");

    mat.verbose=0;
    if(mat.unset) mat_fill_try(&mat);

    printf("total try: %i\n", ntry);
    printf("longest chain: %i\n", lchn);
    printf("\n");

    if(nsol==0) printf("no solution found\n");
    else printf("%i solutions found\n", nsol);

    return 0;
}