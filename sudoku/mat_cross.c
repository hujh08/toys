// analysis overlap lattices of two crossing lattices

#include <stdio.h>

#include "sudoku.h"

int blks_cross(matrix *mat) {
    submat *blks=mat->blks;

    barr_t mark_row=(1<<3)-1,  // mark the lat in same row/col
           mark_col=1+(1<<3)+(1<<6),
           mask_olp=mark_row;   // mask the overlap lattices
    int changed;
    for(int b=0; b<9; b++) {
        if(sub_isset(blks+b)) continue;

        if(mat->verbose) printf("cross in block: %i\n", b);

        int br=b/3,
            bc=b-3*br;

        if(mat->verbose) printf("brc: %i %i\n", br, bc);

        int ms_row=mask_olp<<(3*bc),
            ms_col=mask_olp<<(3*br);
        
        if(mat->verbose) {
            printf("mask overlap in row: ");
            bit_print_bin(ms_row, 9);
            printf("\n");
            printf("mask overlap in col: ");
            bit_print_bin(ms_col, 9);
            printf("\n");
        }

        number_t *nums=blks[b].nums;
        for(int d=0; d<9; d++) {
            // printf("try d: %i\n", d);
            if(cand_isset(nums+d)) continue;

            if(mat->verbose) printf("d: %i\n", d+1);

            barr_t lat_arr=cand_bins(nums+d);
            if(mat->verbose) {
                printf("lat_arr: ");
                bit_print_bin(lat_arr, 9);
                printf("\n");
            }

            int rb, r, c, n, mr=mark_row;
            changed=0;
            if(mat->verbose) printf("search same row\n");
            for(rb=0; rb<3; rb++, mr<<=3) {
                // printf("rb: %i\n", rb);
                if(lat_arr&(~mr)) continue;
                r=3*br+rb;
                if(mat->verbose) {
                    printf("found same row: %i %i\n", rb, r);
                    printf("row mark: ");
                    bit_print_bin(mr, 9);
                    printf("\n");
                }

                for(c=0; c<9; c++) {
                    n=c+9*r;
                    if(ms_row&(1<<c)) continue; // overlap
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;

                        if(mat->verbose)
                            printf("del lat %i, num %i\n", n, d+1);
                        mat_del_latnum(mat, n, d+1);
                    }
                }

                if(changed) {
                    printf("succeed in blk row\n");
                    return SCAN_SUCC;
                }
            }

            int cb, mc=mark_col;
            if(mat->verbose) printf("search same col\n");
            for(cb=0; cb<3; cb++, mc<<=1) {
                // printf("cb: %i\n", cb);
                if(lat_arr&(~mc)) continue;
                c=3*bc+cb;
                if(mat->verbose) {
                    printf("found same col: %i %i\n", cb, c);
                    printf("col mark: ");
                    bit_print_bin(mc, 9);
                    printf("\n");
                }
                for(r=0; r<9; r++) {
                    // printf("r: %i\n", r);
                    n=c+9*r;
                    if(ms_col&(1<<r)) continue; // overlap
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;

                        if(mat->verbose)
                            printf("del lat %i, num %i\n", n, d+1);
                        mat_del_latnum(mat, n, d+1);
                    }
                }
                if(changed) {
                    printf("succeed in blk col\n");
                    return SCAN_SUCC;
                }
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

        if(mat->verbose) printf("cross in rows: %i\n", r);

        int br=r/3,     // block row
            rb=r-3*br;  // row in block

        if(mat->verbose) printf("rb: %i\n", rb);

        int mask=mask_olp<<(3*rb);
        
        if(mat->verbose) {
            printf("mask overlap in block: ");
            bit_print_bin(mask, 9);
            printf("\n");
        }

        number_t *nums=rows[r].nums;
        for(int d=0; d<9; d++) {
            // printf("try d: %i\n", d);
            if(cand_isset(nums+d)) continue;

            if(mat->verbose) printf("d: %i\n", d+1);

            barr_t lat_arr=cand_bins(nums+d);
            if(mat->verbose) {
                printf("lat_arr: ");
                bit_print_bin(lat_arr, 9);
                printf("\n");
            }

            int bc, n, ni, mb=mark_blk;
            changed=0;
            if(mat->verbose) printf("search same block\n");
            for(bc=0; bc<3; bc++, mb<<=3) {
                // printf("rb: %i\n", rb);
                if(lat_arr&(~mb)) continue;
                if(mat->verbose) {
                    printf("found same block: %i %i\n", br, bc);
                    printf("block mark: ");
                    bit_print_bin(mb, 9);
                    printf("\n");
                }

                for(int s=0; s<9; s++) {
                    int j=s/3,
                        i=s-3*j;
                    ni=i+3*j;
                    if(mask&(1<<ni)) continue; // overlap
                    n=i+bc*3+9*(j+br*3);
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;
                        if(mat->verbose)
                            printf("del lat %i, num %i\n", n, d+1);
                        mat_del_latnum(mat, n, d+1);
                    }
                }

                if(changed) {
                    if(mat->verbose) printf("succeed in row blk\n");
                    return SCAN_SUCC;
                }
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

        if(mat->verbose) printf("cross in cols: %i\n", c);

        int bc=c/3,     // block col
            cb=c-3*bc;  // col in block

        if(mat->verbose) printf("cb: %i\n", cb);

        int mask=mask_olp<<cb;
        
        if(mat->verbose) {
            printf("mask overlap in block: ");
            bit_print_bin(mask, 9);
            printf("\n");
        }

        number_t *nums=cols[c].nums;
        for(int d=0; d<9; d++) {
            // printf("try d: %i\n", d);
            if(cand_isset(nums+d)) continue;

            if(mat->verbose) printf("d: %i\n", d+1);

            barr_t lat_arr=cand_bins(nums+d);
            if(mat->verbose) {
                printf("lat_arr: ");
                bit_print_bin(lat_arr, 9);
                printf("\n");
            }

            int br, n, ni, mb=mark_blk;
            changed=0;
            if(mat->verbose) printf("search same block\n");
            for(br=0; br<3; br++, mb<<=3) {
                // printf("rb: %i\n", rb);
                if(lat_arr&(~mb)) continue;
                if(mat->verbose) {
                    printf("found same block: %i %i\n", br, bc);
                    printf("block mark: ");
                    bit_print_bin(mb, 9);
                    printf("\n");
                }

                for(int s=0; s<9; s++) {
                    int j=s/3,
                        i=s-3*j;
                    ni=i+3*j;
                    if(mask&(1<<ni)) continue; // overlap
                    n=i+bc*3+9*(j+br*3);
                    if(lat_isset(mat->lat+n)) continue;
                    if(lat_iscand(mat->lat+n, d+1)) {
                        changed=1;
                        if(mat->verbose)
                            printf("del lat %i, num %i\n", n, d+1);
                        mat_del_latnum(mat, n, d+1);
                    }
                }

                if(changed) {
                    if(mat->verbose) printf("succeed in col blk\n");
                    return SCAN_SUCC;
                }
            }
        }
    }
    return SCAN_NONE;
}

int mat_cross(matrix *mat) {
    int found;

    if(mat->verbose) {
        printf("cross analysis for blks\n");
        print_mat(mat);
    }
    
    found=blks_cross(mat);
    if(found!=SCAN_NONE) return found;
    
    found=rows_cross(mat);
    if(found!=SCAN_NONE) return found;
    
    found=cols_cross(mat);
    if(found!=SCAN_NONE) return found;

    return SCAN_NONE;
}