/*
    extract a group, as a digit, their digits are determined

    this is done after no single lattice could be established

    group in same sub-matrix
*/

#include "sudoku.h"

static fun_id func_ids[]={fid_row, fid_col, fid_blk};

// group for sub-matrixs
int subs_group(matrix *mat, submat *subs, int ts) {
    fun_id f=func_ids[ts];

    int found=0;
    for(int ofi=0; ofi<9; ofi++) {
        if(sub_isset(subs+ofi)) continue;

        cand_t lats[9];
        int unset_lat=0;
        for(int i=0; i<9; i++) {
            lats[i]=mat->lat[f(ofi, i)].cnds;
            if(!cand_isset(lats+i)) unset_lat++;
        }

        number_t *nums=subs[ofi].nums;
        int unset=subs[ofi].unset;

        if(unset!=unset_lat) return SCAN_ERROR;

        barr_t arr, pos;

        // do for collection of numbers
        found=gen_group(nums, lats, unset, &arr, &pos);

        // if(found==SCAN_NONE) continue;
        if(found==SCAN_ERROR) return found;

        if(found==SCAN_SUCC) {
            for(int i=0; i<9; i++) {
                if(!(pos&(1<<i))) continue;

                // cross match in group
                barr_t arr_m=cand_bins(lats+i)&(~arr);

                int n=f(ofi, i);
                for(int d=1; d<=9; d++) {
                    if(!(arr_m&(1<<(d-1)))) continue;
                    mat_del_latnum(mat, n, d);
                }
            }
            print_group(mat, ofi, arr, pos, ts);
            return found;
        }
    }

    return SCAN_NONE;
}

int rows_group(matrix *mat) {
    return subs_group(mat, mat->rows, MARK_ROW);
}

int cols_group(matrix *mat) {
    return subs_group(mat, mat->cols, MARK_COL);
}

int blks_group(matrix *mat) {
    return subs_group(mat, mat->blks, MARK_BLK);
}

int mat_group_sub(matrix *mat) {
    int found;

    found=rows_group(mat);
    if(found!=SCAN_NONE) return found;

    found=cols_group(mat);
    if(found!=SCAN_NONE) return found;

    found=blks_group(mat);
    if(found!=SCAN_NONE) return found;

    return SCAN_NONE;
}