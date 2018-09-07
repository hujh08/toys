/*
    simple solver for sudoku
    just do analysis for each lattice, and try
*/

#include <stdio.h>

#include "sudoku.h"

int main(int argc, char **argv)
{
    char *filename="input.txt";

    printf("input: %s\n", filename);

    matrix mat;
    readtxt(filename, &mat);

    printf("unset lattices: %i\n", mat.unset);
    printf("initial state:\n");
    print_mat(&mat);

    int n=1;
    printf("state for lat %i\n", n+1);
    print_lat(mat.lat+n);

    mat_analysis(&mat);

    print_lat(mat.lat+n);

    // for(int i=0; i<81; i++) {
    //     if(!lat_isset(mat.lat+i)) {
    //         printf("lat %i\n", i+1);
    //         print_lat(mat.lat+i);
    //         printf("\n");
    //     }
    // }

    nsol=0;

    mat_fill_notry(&mat);

    printf("matrix after fill without try\n");
    printf("unset lattices: %i\n", mat.unset);
    print_mat(&mat);

    if(mat.unset) mat_fill_try(&mat);

    if(nsol==0) printf("no solution found\n");
    
    return 0;
}