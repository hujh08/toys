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

    parse_args(argc, argv, &mat);

    mat_init_unset(&mat);
    // printf("empty state:\n");
    // print_mat(&mat);

    // int n=2;
    // printf("row %i\n", n+1);
    // print_sub(mat.rows+n);
    // printf("\n");

    readtxt(filename, &mat);

    print_sub(mat.blks+1);
    printf("\n");

    printf("unset lattices: %i\n", mat.unset);
    printf("initial state:\n");
    print_mat(&mat);

    // printf("state for lat %i\n", n+1);
    // print_lat(mat.lat+n);

    // mat_analysis(&mat);

    // print_lat(mat.lat+n);
    // printf("\n");

    // n=2;
    // printf("row %i\n", n+1);
    // print_sub(mat.rows+n);
    // printf("\n");

    // n=0;
    // printf("col %i\n", n+1);
    // print_sub(mat.cols+n);

    // n=0;
    // printf("block %i\n", n+1);
    // print_sub(mat.blks+n);

    // for(int i=0; i<81; i++) {
    //     if(!lat_isset(mat.lat+i)) {
    //         printf("lat %i\n", i+1);
    //         print_lat(mat.lat+i);
    //         printf("\n");
    //     }
    // }

    nsol=0;
    ntry=0;

    int found=mat_fill_notry(&mat);

    printf("after fill without try:\n");
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

    // n=0;
    // print_sub(mat.blks+n);

    mat.verbose=0;
    if(mat.unset) mat_fill_try(&mat);

    printf("total try: %i\n", ntry);

    if(nsol==0) printf("no solution found\n");
    else printf("%i solutions found\n", nsol);


    // printf("\n");
    // printf("right shift: %i\n", 1<<0);
    // printf("right shift: %i\n", 1<<2);

    // bitarr bta;
    // printf("max length: %li\n", MAXLEN);
    // printf("clear bit array\n");
    // bit_clear(&bta);
    // bit_print(&bta);

    // printf("fill to length 4\n");
    // bit_fill(&bta, 4);
    // bit_print(&bta);

    // printf("unset 2\n");
    // bit_unset(&bta, 2);
    // bit_print(&bta);

    // printf("set 15\n");
    // bit_set(&bta, 15);
    // bit_print(&bta);
    
    return 0;
}