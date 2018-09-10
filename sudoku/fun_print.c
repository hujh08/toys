// print function

#include <stdio.h>

#include "sudoku.h"

// print lattice
void print_lat(lattice *lat) {
    if(lat_isset(lat)) {
        printf("set\n");
        printf("number: %i\n", lat->number);
    } else {
        printf("unset\n");
        printf("number of possible: %i\n", lat_cand_num(lat));
        printf("candidate:");
        for(int i=1; i<=9; i++) {
            if(lat_iscand(lat, i)) printf(" %i", i);
        }
        printf("\n");
    }
}

// print sub-matrix
void print_sub(submat *sub) {
    if(sub->unset==0) {
        printf("sub-matrix is set down\n");
        return;
    }

    number_t *nums=sub->nums;

    printf("%i set numbers:", 9-sub->unset);
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) printf(" %i", i+1);
    }
    printf("\n");
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) continue;
        printf("number %i: %i lat", i+1, cand_num(nums+i));
        if(!cand_isset(nums+i)) {
            for(int n=0; n<9; n++) {
                if(bit_has(&(nums[i].arr), n))
                    printf(" %i", n+1);
            }
            printf("\n");
        }
    }
}

// print matrix
void print_mat(matrix *mat) {
    lattice *lat=mat->lat;
    int n=0;
    for(int j=0; j<9; j++) {
        if(j==3 || j==6) printf("\n");
        printf("       ");
        for(int i=0; i<9; i++, n++) {
            if(i==3 || i==6) printf("  ");
            if(lat_isset(lat+n)) printf(" %i", lat[n].number);
            else printf(" x");
        }
        printf("\n");
    }
    printf("\n");
}

// print finished matrix
void print_result(matrix *mat) {
    printf("solution %i:\n", ++nsol);
    printf("total try: %i\n", ntry);
    printf("level of try: %i\n", mat->ntry);
    print_mat(mat);
}