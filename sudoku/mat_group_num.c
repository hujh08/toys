/*
    do group in blk-row/col plane with same number candidate

    this must be done after mat_cross
*/

#include <stdio.h>

#include "sudoku.h"

// static fun_id func_ids[]={fid_row, fid_col, fid_blk};

static fun_subid func_subids[]={fsubid_row, fsubid_col, fsubid_blk};

// extract sub-sub plane with same digit
int ext_subsub(lattice *lats, int d, fun_subid f0, fun_subid f1,
                    cand_t *cnds0, cand_t *cnds1) {
    // d starts from 0
    // printf("ext subsub for num %i\n", d+1);
    // initiate
    for(int b=0; b<9; b++) cand_init_unset_empty(cnds0+b);
    for(int b=0; b<9; b++) cand_init_unset_empty(cnds1+b);

    int n0, n1, tmp;
    for(int n=0; n<81; n++) {
        f0(n, &n0, &tmp);
        f1(n, &n1, &tmp);

        if(lat_isnum(lats+n, d+1)) cnds0[n0].set=cnds1[n1].set=1;
        if(lat_isset(lats+n)) continue;
        if(lat_iscand(lats+n, d+1)) {
            // printf("got %i: %i %i\n", n, n0+1, n1+1);
            cand_set(cnds0+n0, n1);
            cand_set(cnds1+n1, n0);
        }
    }

    int unset0=0, unset1=0;
    for(int i=0; i<9; i++) {
        if(!cand_isset(cnds0+i)) unset0++;
        if(!cand_isset(cnds1+i)) unset1++;
    }

    if(unset0!=unset1) return SCAN_ERROR;

    return unset0;
}

int mat_group_subsub(matrix *mat, int t0, int t1) {
    // printf("group subsub: %i %i\n", t0, t1);
    // print_mat(mat);
    fun_subid f0=func_subids[t0],
              f1=func_subids[t1];

    lattice *lats=mat->lat;

    cand_t cnds0[9], cnds1[9];
    int unset, maxs, found;

    for(int d=0; d<9; d++) {
        unset=ext_subsub(lats, d, f0, f1, cnds0, cnds1);

        if(unset==0) continue;

        // printf("unset: %i\n", unset);

        if(unset<0) return SCAN_ERROR;

        barr_t pos0, pos1;

        if(t0==MARK_BLK || t1==MARK_BLK) maxs=2;
        else maxs=unset-1;

        // do for collection of numbers
        found=gen_group(cnds0, cnds1, maxs, &pos0, &pos1);

        if(found==SCAN_ERROR) return found;

        if(found==SCAN_SUCC) {
            int n0, n1, tmp;
            for(int n=0; n<81; n++) {
                f0(n, &n0, &tmp);
                f1(n, &n1, &tmp);

                if(pos0&(1<<n0)) continue;
                if(!(pos1&(1<<n1))) continue;

                if(lat_isset(lats+n)) continue;
                if(!lat_iscand(lats+n, d+1)) continue;

                mat_del_latnum(mat, n, d+1);
            }
            print_group(mat, d, pos0, pos1, t0, t1);
            return found;
        }
    }
    return SCAN_NONE;
}

int mat_group_num(matrix *mat) {
    int found;

    found=mat_group_subsub(mat, MARK_ROW, MARK_COL);
    if(found!=SCAN_NONE) return found;

    found=mat_group_subsub(mat, MARK_ROW, MARK_BLK);
    if(found!=SCAN_NONE) return found;

    found=mat_group_subsub(mat, MARK_COL, MARK_BLK);
    if(found!=SCAN_NONE) return found;

    return SCAN_NONE;
}
