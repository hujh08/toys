// analysis overlap lattices of two crossing lattices

#include "sudoku.h"

int blks_cross(matrix *mat) {
    submat *blks=mat->blks;

    barr_t mark_row=(1<<3)-1,  // mark the lat in same row/col
           mark_col=1+(1<<3)+(1<<6),
           mask_olp=mark_row;   // mask the overlap lattices
    int changed;
    for(int b=0; b<9; b++) {
        if(sub_isset(blks+b)) continue;

        int br=b/3,
            bc=b-3*br;

        int ms_row=mask_olp<<(3*bc),
            ms_col=mask_olp<<(3*br);
        
        number_t *nums=blks[b].nums;
        for(int d=0; d<9; d++) {
            if(cand_isset(nums+d)) continue;

            barr_t lat_arr=cand_bins(nums+d);

            int rb, r, c, n, mr=mark_row;
            changed=0;
            for(rb=0; rb<3; rb++, mr<<=3) {
                if(lat_arr&(~mr)) continue;

                r=3*br+rb;
                for(c=0; c<9; c++) {
                    n=c+9*r;
                    if(ms_row&(1<<c)) continue; // overlap
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;
                        mat_del_latnum(mat, n, d+1);
                    }
                }

                if(changed) return SCAN_SUCC;
            }

            int cb, mc=mark_col;
            for(cb=0; cb<3; cb++, mc<<=1) {
                if(lat_arr&(~mc)) continue;

                c=3*bc+cb;
                for(r=0; r<9; r++) {
                    n=c+9*r;
                    if(ms_col&(1<<r)) continue; // overlap
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;
                        mat_del_latnum(mat, n, d+1);
                    }
                }
                if(changed) return SCAN_SUCC;
            }
        }
    }
    return SCAN_NONE;
}

int rows_cross(matrix *mat) {
    submat *rows=mat->rows;

    barr_t mark_blk=(1<<3)-1,  // mark the lat in same block
           mask_olp=(1<<3)-1;   // mask the overlap lattices
    int changed;
    for(int r=0; r<9; r++) {
        if(sub_isset(rows+r)) continue;

        int br=r/3,     // block row
            rb=r-3*br;  // row in block

        int mask=mask_olp<<(3*rb);
        
        number_t *nums=rows[r].nums;
        for(int d=0; d<9; d++) {
            if(cand_isset(nums+d)) continue;

            barr_t lat_arr=cand_bins(nums+d);

            int bc, n, ni, mb=mark_blk;
            changed=0;
            for(bc=0; bc<3; bc++, mb<<=3) {
                if(lat_arr&(~mb)) continue;

                for(int s=0; s<9; s++) {
                    int j=s/3,
                        i=s-3*j;
                    ni=i+3*j;
                    if(mask&(1<<ni)) continue; // overlap
                    n=i+bc*3+9*(j+br*3);
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;
                        mat_del_latnum(mat, n, d+1);
                    }
                }

                if(changed) return SCAN_SUCC;
            }
        }
    }
    return SCAN_NONE;
}

int cols_cross(matrix *mat) {
    submat *cols=mat->cols;

    barr_t mark_blk=(1<<3)-1,  // mark the lat in same block
           mask_olp=1+(1<<3)+(1<<6);   // mask the overlap lattices
    int changed;
    for(int c=0; c<9; c++) {
        if(sub_isset(cols+c)) continue;

        int bc=c/3,     // block col
            cb=c-3*bc;  // col in block

        int mask=mask_olp<<cb;

        number_t *nums=cols[c].nums;
        for(int d=0; d<9; d++) {
            if(cand_isset(nums+d)) continue;

            barr_t lat_arr=cand_bins(nums+d);

            int br, n, ni, mb=mark_blk;
            changed=0;
            for(br=0; br<3; br++, mb<<=3) {
                if(lat_arr&(~mb)) continue;

                for(int s=0; s<9; s++) {
                    int j=s/3,
                        i=s-3*j;
                    ni=i+3*j;
                    if(mask&(1<<ni)) continue; // overlap
                    n=i+bc*3+9*(j+br*3);
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;
                        mat_del_latnum(mat, n, d+1);
                    }
                }

                if(changed) return SCAN_SUCC;
            }
        }
    }
    return SCAN_NONE;
}

int mat_cross(matrix *mat) {
    int found;

    found=blks_cross(mat);
    if(found!=SCAN_NONE) return found;
    
    found=rows_cross(mat);
    if(found!=SCAN_NONE) return found;
    
    found=cols_cross(mat);
    if(found!=SCAN_NONE) return found;

    return SCAN_NONE;
}