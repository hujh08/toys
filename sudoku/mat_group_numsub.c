/*
    extract a group, as a digit, their digits are determined

    this is done after no single lattice could be established
*/

#include <stdio.h>

#include "sudoku.h"

// group for sub-matrixs
int subs_group(matrix *mat, submat *subs, fun_id f) {
    int verb=mat->verbose;
    if(verb) {
        printf("\nmat group in number-sub plane\n");

        print_mat(mat);
    }

    int found=0;
    for(int ofi=0; ofi<9; ofi++) {
        if(sub_isset(subs+ofi)) continue;

        // if(verb) {
        //     printf("sub: %i\n", ofi);
        //     print_sub(subs+ofi);
        // }

        cand_t lats[9];
        int unset_lat=0;
        for(int i=0; i<9; i++) {
            lats[i]=mat->lat[f(ofi, i)].cnds;
            if(!cand_isset(lats+i)) unset_lat++;
        }

        number_t *nums=subs[ofi].nums;
        int unset=subs[ofi].unset;

        if(unset!=unset_lat) {
            if(verb) printf("unequal for unset: %i %i\n", 
                                    unset, unset_lat);
            return SCAN_ERROR;
        }

        barr_t arr, pos;

        // do for collection of numbers
        // if(verb) printf("numbers group for sub %i\n", ofi);
        found=gen_group(nums, lats, unset, 0, &arr, &pos);

        // if(found==SCAN_NONE) continue;
        if(found==SCAN_ERROR) return found;

        if(found==SCAN_SUCC) {
            if(verb) {
                printf("succeed in num-sub plane\n");

                printf("numbers group for sub %i\n", ofi);
                printf("arr: ");
                bit_print_bin(arr, 9);
                printf("\n");

                printf("pos: ");
                bit_print_bin(pos, 9);
                printf("\n");
            }

            for(int i=0; i<9; i++) {
                if(!(pos&(1<<i))) continue;

                if(verb) {
                    printf("lat: %i\n", i);
                    printf("   original: ");
                    bit_print_bin(cand_bins(lats+i), 9);
                    printf("\n");
                }
            
                // cross match in group
                barr_t arr_m=cand_bins(lats+i)&(~arr);

                if(verb) {
                    printf("        arr: ");
                    bit_print_bin(arr, 9);
                    printf("\n");
                    printf("cross match: ");
                    bit_print_bin(arr_m, 9);
                    printf("\n");
                }

                int n=f(ofi, i);
                for(int d=1; d<=9; d++) {
                    if(!(arr_m&(1<<(d-1)))) continue;
                    if(verb)
                        printf("del lat %i, num %i\n", n, d);

                    mat_del_latnum(mat, n, d);
                }
            }
            return found;
        }
    }

    return SCAN_NONE;
}

int rows_group(matrix *mat) {
    return subs_group(mat, mat->rows, fid_row);
}

int cols_group(matrix *mat) {
    return subs_group(mat, mat->cols, fid_col);
}

int blks_group(matrix *mat) {
    return subs_group(mat, mat->blks, fid_blk);
}

int mat_group_numsub(matrix *mat) {
    int found;

    if(mat->verbose) printf("group for rows\n");
    found=rows_group(mat);
    if(found!=SCAN_NONE) return found;

    if(mat->verbose) printf("group for cols\n");
    found=cols_group(mat);
    if(found!=SCAN_NONE) return found;

    if(mat->verbose) printf("group for blks\n");
    found=blks_group(mat);
    if(found!=SCAN_NONE) return found;

    return SCAN_NONE;
}