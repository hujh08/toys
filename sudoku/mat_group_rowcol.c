/*
    do group in row-col plane with same number candidate
*/

#include <stdio.h>

#include "sudoku.h"

int mat_group_rowcol(matrix *mat) {
    int verb=mat->verbose;
    if(verb) {
        printf("\nmat group in row-col plane\n");

        print_mat(mat);
    }

    submat *rows=mat->rows,
           *cols=mat->cols;

    cand_t rows_num[9],  // candidate for same number in rows/cols
           cols_num[9];
    int unset_row, unset_col;
    int found=0;
    for(int d=0; d<9; d++) {
        unset_row=unset_col=0;
        for(int i=0; i<9; i++) {
            rows_num[i]=rows[i].nums[d];
            cols_num[i]=cols[i].nums[d];

            if(!cand_isset(rows_num+i)) unset_row++;
            if(!cand_isset(cols_num+i)) unset_col++;
        }

        if(unset_row!=unset_col) {
            if(verb) printf("unequal row/col unset: %i %i\n", 
                                    unset_row, unset_col);
            return SCAN_ERROR;
        }

        barr_t pos_row, pos_col;

        // do for collection of numbers
        // if(verb) printf("numbers group for sub %i\n", ofi);
        found=gen_group(rows_num, cols_num, unset_row, 0, 
                        &pos_row, &pos_col);

        // if(found==SCAN_NONE) continue;
        if(found==SCAN_ERROR) return found;

        if(found==SCAN_SUCC) {
            if(verb) {
                printf("succeed in row-col plane\n");
                printf("group for number %i\n", d);
                printf("pos in row: ");
                bit_print_bin(pos_row, 9);
                printf("\n");

                printf("pos in col: ");
                bit_print_bin(pos_col, 9);
                printf("\n");
            }

            for(int r=0; r<9; r++) {
                if(pos_col&(1<<r)) continue;

                barr_t pos_r=cand_bins(rows_num+r),
                       pos_m=pos_r&pos_col;
                if(verb) {
                    printf("row: %i\n", r);

                    printf("   original: ");
                    bit_print_bin(pos_r, 9);
                    printf("\n");

                    printf("    pos col: ");
                    bit_print_bin(pos_col, 9);
                    printf("\n");

                    printf("cross match: ");
                    bit_print_bin(pos_m, 9);
                    printf("\n");
                }

                for(int c=0; c<9; c++) {
                    if(!(pos_m&(1<<c))) continue;
                    int n=c+9*r;
                    if(verb)
                        printf("del lat %i, num %i\n", n, d+1);

                    mat_del_latnum(mat, n, d+1);
                }
            }
            return found;
        }
    }

    return SCAN_NONE;
}
