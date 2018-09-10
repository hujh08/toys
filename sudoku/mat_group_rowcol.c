/*
    do group in row-col plane with same number candidate
*/

#include "sudoku.h"

int mat_group_rowcol(matrix *mat) {
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

        if(unset_row!=unset_col) return SCAN_ERROR;

        barr_t pos_row, pos_col;

        // do for collection of numbers
        found=gen_group(rows_num, cols_num, unset_row,
                        &pos_row, &pos_col);

        // if(found==SCAN_NONE) continue;
        if(found==SCAN_ERROR) return found;

        if(found==SCAN_SUCC) {
            for(int r=0; r<9; r++) {
                if(pos_col&(1<<r)) continue;

                barr_t pos_r=cand_bins(rows_num+r),
                       pos_m=pos_r&pos_col;
                for(int c=0; c<9; c++) {
                    if(!(pos_m&(1<<c))) continue;
                    int n=c+9*r;
                    mat_del_latnum(mat, n, d+1);
                }
            }
            return found;
        }
    }

    return SCAN_NONE;
}
