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

int scan_rows(matrix *mat, int *n, int *d) {
    submat *rows=mat->rows;
    for(int r=0; r<9; r++) {
        if(sub_isset(rows+r)) continue;

        number_t *nums=rows[r].nums;
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
                int c=num_1stcand(nums+i);
                *n=c+9*r;
                *d=i+1;
                return SCAN_SUCC;
            }
        }
    }

    return SCAN_NONE;
}

int scan_cols(matrix *mat, int *n, int *d) {
    // printf("scan cols\n");
    submat *cols=mat->cols;
    for(int c=0; c<9; c++) {
        // if(c==0) printf("0 cols: %i\n", sub_isset(cols+c));
        if(sub_isset(cols+c)) continue;

        number_t *nums=cols[c].nums;
        for(int i=0; i<9; i++) {
            // if(c==0 && i==0)
                // printf("0 cols 0 num: %i\n", cand_isset(nums+i));

            if(cand_isset(nums+i)) continue;

            int nc=cand_num(nums+i);
            if(nc==0) return SCAN_ERROR;
            if(nc==1) {
                int r=num_1stcand(nums+i);
                *n=c+9*r;
                *d=i+1;
                return SCAN_SUCC;
            }
        }
    }

    return SCAN_NONE;
}

int scan_blks(matrix *mat, int *n, int *d) {
    submat *blks=mat->blks;
    for(int b=0; b<9; b++) {
        if(sub_isset(blks+b)) continue;

        number_t *nums=blks[b].nums;
        for(int i=0; i<9; i++) {
            if(cand_isset(nums+i)) continue;

            int nc=cand_num(nums+i);
            if(nc==0) return SCAN_ERROR;
            if(nc==1) {
                int s=num_1stcand(nums+i);
                *n=(3*(b%3)+(s%3))+9*(3*(b/3)+(s/3));
                *d=i+1;
                return SCAN_SUCC;
            }
        }
    }

    return SCAN_NONE;
}