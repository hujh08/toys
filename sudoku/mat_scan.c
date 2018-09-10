// scan matrix to find out a lattice to fill

#include <stdio.h>

#include "sudoku.h"

// return if found lattice
int scan_lats(matrix *mat, int *n, int *d) {
    lattice *lat=mat->lat;

    for(int i=0; i<81; i++) {
        if(lat_isset(lat+i)) continue;
        int nc=lat_cand_num(lat+i);  // number of candidates
        // if(toset==32) printf("num: %i\n", nc);
        if(nc==0) return SCAN_ERROR;
        if(nc==1) {
            *n=i;
            *d=lat_1stcand(lat+i);
            return SCAN_SUCC;
        }
    }

    return SCAN_NONE;
}

// general function to scan sub-matrix
int scan_sub(number_t *nums, int *insub, int *digit) {
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) continue;

        int nc=cand_num(nums+i);
        if(nc==0) {
            // printf("%i row error\n", r+1);
            // print_sub(rows+r);
            // print_mat(mat);
            return SCAN_ERROR;
        }
        if(nc==1) {
            *insub=cand_1stcand(nums+i);
            *digit=i+1;
            return SCAN_SUCC;
        }
    }
    return SCAN_NONE;
}

// general function for subs
int scan_subs(submat *subs, int *n, int *d, fun_id f) {
    int found;
    for(int i=0; i<9; i++) {
        if(sub_isset(subs+i)) continue;

        number_t *nums=subs[i].nums;

        int j;
        found=scan_sub(nums, &j, d);

        if(found==SCAN_NONE) continue;
        if(found==SCAN_ERROR) return found;

        *n=f(i, j);
        return SCAN_SUCC;
    }

    return SCAN_NONE;
}

// scan rows/cols/blocks
int scan_rows(matrix *mat, int *n, int *d) {
    submat *rows=mat->rows;
    return scan_subs(rows, n, d, fid_row);
}

int scan_cols(matrix *mat, int *n, int *d) {
    // printf("scan cols\n");
    submat *cols=mat->cols;
    return scan_subs(cols, n, d, fid_col);
}

int scan_blks(matrix *mat, int *n, int *d) {
    submat *blks=mat->blks;
    return scan_subs(blks, n, d, fid_blk);
}