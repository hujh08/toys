/*
    extract a group, as a digit, their digits are determined

    this is done after no single lattice could be established
*/

#include <stdio.h>

#include "sudoku.h"

// combine function: select m from n
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

// general function to do clustering
int gen_group(cand_t *sub, cand_t *trans, int unset, int verbose,
                    barr_t *sub_grp, barr_t *sub_pos) {
    /*
        parameter `sub` is a collection of candidates
            of numbers/lattices in a sub structure, row/col/blk
        which looks like a binary matrix

        so if sub is collection of numbers, `trans` is of lattices

        !!these two type of sub is equivalent!!

        !!the matrix could also be rows and cols with same number
            candidate!!

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
