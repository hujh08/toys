/*
    extract a group, as a digit, their digits are determined

    this is done after no single lattice could be established
*/

#include <stdio.h>

#include "sudoku.h"

// combine function: select m from n
// #define COMB_ERR -1
// #define COMB_SUC 0
int comb_next(int arr, int n) {
    // int arr=*now;
        // arr_max=arr<<(n-m);

    // if(arr>=arr_max) return COMB_ERR;

    int i;
    for(i=0; i<n-1; i++) if(((arr>>i)&3)==1) break;

    arr^=(3<<i);

    int mask=(1<<i)-1,
        arr_h=arr&(~mask),
        arr_l=arr&mask;
    if(arr_l) {
        while((arr_l&1)==0) arr_l>>=1;
    }
    arr=arr_h|arr_l;

    return arr;
}

// do clustering for sub-matrix
int sub_group(cand_t *sub, cand_t *trans, int unset, int verbose,
                    barr_t *sub_grp, barr_t *sub_pos) {
    /*
        parameter `sub` is a collection of candidates
            of numbers/lattices in a sub structure, row/col/blk
        which looks like a binary matrix

        so if sub is collection of numbers, `trans` is of lattices

        in the following comments, sub is treated as of numbers
            so trans is of lattice
    */
    if(verbose) printf("group for single sub\n");

    int mins=9;
    
    barr_t mask_grp=0,        // mask grouped lattice
           mask_cnt=(1<<9)-1;

    for(int i=0; i<9; i++) {
        if(cand_isset(sub+i)) continue;
        int n=cand_num(sub+i);
        if(n<mins) mins=n;
    }

    if(verbose) printf("min group size: %i\n", mins);

    for(int size=mins; size<unset; size++) {
        if(verbose) printf("group size: %i\n", size);
        barr_t arr=(1<<size)-1,
               arr_max=arr<<(9-size);

        for(int i=0; i<9; i++) {
            if(cand_isset(sub+i)) continue;
            if(cand_num(sub+i)==size) mask_cnt&=~(1<<i);
        }


        if(verbose) {
            printf("group mask: ");
            bit_print_bin(mask_grp, 9);
            printf("\n");

            printf("count mask: ");
            bit_print_bin(mask_cnt, 9);
            printf("\n");
        }

        for(;arr<=arr_max; arr=comb_next(arr, 9)) {
            if(arr&(mask_grp|mask_cnt)) continue;

            if(verbose) printf("\n");

            if(verbose) {
                printf("arr: ");
                bit_print_bin(arr, 9);
                printf("\n");
            }

            // union cands
            barr_t cnds=0;
            for(int i=0; i<9; i++) {
                if(arr&(1<<i)) cnds|=cand_bins(sub+i);
            }

            if(verbose) {
                printf("union cands: ");
                bit_print_bin(cnds, 9);
                printf("\n");
            }

            int num_u=bit_num_bin(cnds, 9);
            if(verbose) printf("group size: %i\n", size);
            if(verbose) printf("union size: %i\n", num_u);

            if(num_u>size) continue;
            if(num_u<size) return SCAN_ERROR;

            if(verbose) printf("found group\n");

            *sub_pos=0;
            for(int i=0; i<9; i++) {
                if(cand_isset(trans+i)) continue;

                if(verbose) {
                    printf("lat: %i\n", i);
                    printf("   original: ");
                    bit_print_bin(cand_bins(trans+i), 9);
                    printf("\n");
                }
                if(!(cnds&(1<<i))) continue;

                // cross match in group
                barr_t arr_m=cand_bins(trans+i)&(~arr);
                if(!arr_m) continue;

                if(verbose) {
                    printf("        arr: ");
                    bit_print_bin(arr, 9);
                    printf("\n");
                    printf("cross match: ");
                    bit_print_bin(arr_m, 9);
                    printf("\n");
                }

                *sub_pos|=1<<i;
            }

            if(*sub_pos) {
                if(verbose) printf("group succeed\n");
                *sub_grp=arr;
                return SCAN_SUCC;
            }

            mask_grp|=arr;

            if(verbose) printf("\n");
        }

        if(verbose) printf("\n");
    }

    return SCAN_NONE;
}

// type of function convert id of sub to rcbs
//      which is id of row, col, block and inside block
typedef void (*fun_nrcbs)(int , int , int *, int *);

void frcbs_n(int n, int *rcbs) {
    rcbs[0]=n/9;
    rcbs[1]=n-rcbs[0]*9;
    rcbs[2]=NOBLK(rcbs[0], rcbs[1]);
    rcbs[3]=NIBLK(rcbs[0], rcbs[1]);
}

void fnrcbs_row(int ofsub, int insub, int *n, int *rcbs) {
    *n=fid_row(ofsub, insub);
    if(rcbs) frcbs_n(*n, rcbs);
}

void fnrcbs_col(int ofsub, int insub, int *n, int *rcbs) {
    *n=fid_col(ofsub, insub);
    if(rcbs) frcbs_n(*n, rcbs);
}

void fnrcbs_blk(int ofsub, int insub, int *n, int *rcbs) {
    *n=fid_blk(ofsub, insub);
    if(rcbs) frcbs_n(*n, rcbs);
}

// group for sub-matrixs
int subs_group(matrix *mat, submat *subs, fun_nrcbs f) {
    int verb=mat->verbose;
    if(verb) {
        printf("\nmat group\n");

        print_mat(mat);
    }

    int found=0, nlat;
    for(int ofi=0; ofi<9; ofi++) {
        if(sub_isset(subs+ofi)) continue;

        // if(verb) {
        //     printf("sub: %i\n", ofi);
        //     print_sub(subs+ofi);
        // }

        cand_t lats[9];
        int unset_lat=0;
        for(int i=0; i<9; i++) {
            f(ofi, i, &nlat, NULL);
            lats[i]=mat->lat[nlat].cnds;
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
        found=sub_group(nums, lats, unset, 0, &arr, &pos);

        // if(found==SCAN_NONE) continue;
        if(found==SCAN_ERROR) return found;

        if(found==SCAN_SUCC) {
            if(verb) {
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

                int rcbs[4];
                f(ofi, i, &nlat, rcbs);
                // int n=fid_blk(b, i);
                // int r=n/3,
                //     c=n-r*3;
                int r=rcbs[0], c=rcbs[1],
                    b=rcbs[2], s=rcbs[3],
                    n=9*r+c;

                for(int d=1; d<=9; d++) {
                    if(!(arr_m&(1<<(d-1)))) continue;
                    if(verb)
                        printf("del lat %i, num %i\n", n, d);

                    lat_exclude(mat->lat+n, d);

                    sub_del_latnum(mat->rows+r, c, d);
                    sub_del_latnum(mat->cols+c, r, d);
                    sub_del_latnum(mat->blks+b, s, d);
                }
            }
        return found;
        }

        // // do for collection of lattices
        // if(verb) printf("do group in lattices\n");
        // found=sub_group(lats, nums, unset, verb, &arr, &pos);

        // // if(found==SCAN_NONE) continue;
        // if(found==SCAN_ERROR) return found;

        // if(found==SCAN_SUCC) {
        //     for(int i=0; i<9; i++) {
        //         if(arr&(1<<i)) continue;

        //         if(verb) {
        //             printf("lat: %i\n", i);
        //             printf("   original: ");
        //             bit_print_bin(cand_bins(lats+i), 9);
        //             printf("\n");
        //         }
            
        //         // cross match in group
        //         barr_t arr_m=cand_bins(lats+i)&pos;

        //         if(verb) {
        //             printf("        arr: ");
        //             bit_print_bin(arr, 9);
        //             printf("\n");
        //             printf("cross match: ");
        //             bit_print_bin(arr_m, 9);
        //             printf("\n");
        //         }

        //         int rcbs[4];
        //         f(ofi, i, &nlat, rcbs);
        //         // int n=fid_blk(b, i);
        //         // int r=n/3,
        //         //     c=n-r*3;
        //         int r=rcbs[0], c=rcbs[1],
        //             b=rcbs[2], s=rcbs[3],
        //             n=9*r+c;

        //         for(int d=1; d<=9; d++) {
        //             if(!(arr_m&(1<<(d-1)))) continue;
        //             if(verb)
        //                 printf("del num %i, lat %i\n", d, n);

        //             lat_exclude(mat->lat+n, d);

        //             sub_del_latnum(mat->rows+r, c, d);
        //             sub_del_latnum(mat->cols+c, r, d);
        //             sub_del_latnum(mat->blks+b, s, d);
        //         }
        //     }
        // return found;
        // }

    }

    return SCAN_NONE;
}

int rows_group(matrix *mat) {
    return subs_group(mat, mat->rows, fnrcbs_row);
}

int cols_group(matrix *mat) {
    return subs_group(mat, mat->cols, fnrcbs_col);
}

int blks_group(matrix *mat) {
    return subs_group(mat, mat->blks, fnrcbs_blk);
}

int mat_group(matrix *mat) {
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

    // int verb=mat->verbose;
    // if(verb) printf("\nmat group\n");

    // lattice *mlat=mat->lat;
    // submat *blks=mat->blks;

    // int found=0;
    // for(int b=0; b<9; b++) {
    //     if(sub_isset(blks+b)) continue;

    //     if(verb) {
    //         printf("block: %i\n", b);
    //         print_sub(blks+b);
    //     }

    //     cand_t lats[9];
    //     int unset_lat=0;
    //     for(int i=0; i<9; i++) {
    //         lats[i]=mlat[fid_blk(b, i)].cnds;
    //         if(!cand_isset(lats+i)) unset_lat++;
    //     }

    //     number_t *nums=blks[b].nums;
    //     int unset=blks[b].unset;

    //     if(unset!=unset_lat) return SCAN_ERROR;

    //     barr_t arr, pos;

    //     // do for collection of numbers
    //     if(verb) printf("do group in numbers\n");
    //     found=sub_group(nums, lats, unset, verb, &arr, &pos);

    //     // if(found==SCAN_NONE) continue;
    //     if(found==SCAN_ERROR) return found;

    //     if(found==SCAN_SUCC) {
    //         for(int i=0; i<9; i++) {
    //             if(!(pos&(1<<i))) continue;

    //             if(verb) {
    //                 printf("lat: %i\n", i);
    //                 printf("   original: ");
    //                 bit_print_bin(cand_bins(lats+i), 9);
    //                 printf("\n");
    //             }
            
    //             // cross match in group
    //             barr_t arr_m=cand_bins(lats+i)&(~arr);

    //             if(verb) {
    //                 printf("        arr: ");
    //                 bit_print_bin(arr, 9);
    //                 printf("\n");
    //                 printf("cross match: ");
    //                 bit_print_bin(arr_m, 9);
    //                 printf("\n");
    //             }

    //             int rcbs[4];
    //             frcbs_blk(b, i, rcbs);
    //             // int n=fid_blk(b, i);
    //             // int r=n/3,
    //             //     c=n-r*3;
    //             int r=rcbs[0], c=rcbs[1];

    //             for(int d=1; d<=9; d++) {
    //                 if(!(arr_m&(1<<(d-1)))) continue;
    //                 if(verb)
    //                     printf("del num %i, lat %i\n", d, i);

    //                 lat_exclude(mat->lat+i, d);

    //                 sub_del_latnum(mat->rows+r, c, d);
    //                 sub_del_latnum(mat->cols+c, r, d);
    //                 sub_del_latnum(mat->blks+b, i, d);
    //             }
    //         }
    //     return found;
    //     }

    //     // // do for collection of lattices
    //     // if(verb) printf("do group in lattices\n");
    //     // found=sub_group(lats, nums, unset, verb, &arr, &pos);

    //     // // if(found==SCAN_NONE) continue;
    //     // if(found==SCAN_ERROR) return found;

    //     // if(found==SCAN_SUCC) {
    //     //     for(int i=0; i<9; i++) {
    //     //         if(arr&(1<<i)) continue;

    //     //         if(verb) {
    //     //             printf("lat: %i\n", i);
    //     //             printf("   original: ");
    //     //             bit_print_bin(cand_bins(lats+i), 9);
    //     //             printf("\n");
    //     //         }
            
    //     //         // cross match in group
    //     //         barr_t arr_m=cand_bins(lats+i)&pos;

    //     //         if(verb) {
    //     //             printf("        arr: ");
    //     //             bit_print_bin(arr, 9);
    //     //             printf("\n");
    //     //             printf("cross match: ");
    //     //             bit_print_bin(arr_m, 9);
    //     //             printf("\n");
    //     //         }

    //     //         int n=fid_blk(b, i);
    //     //         int r=n/3,
    //     //             c=n-r*3;

    //     //         for(int d=1; d<=9; d++) {
    //     //             if(!(arr_m&(1<<(d-1)))) continue;
    //     //             if(verb)
    //     //                 printf("del num %i, lat %i\n", d, i);

    //     //             lat_exclude(mat->lat+i, d);

    //     //             sub_del_latnum(mat->rows+r, c, d);
    //     //             sub_del_latnum(mat->cols+c, r, d);
    //     //             sub_del_latnum(mat->blks+b, i, d);
    //     //         }
    //     //     }
    //     // return found;
    //     // }

    // }

    // return SCAN_NONE;
}