/*
    extract a group, as a digit, their digits are determined

    this is done after no single lattice could be established
*/

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
int gen_group(cand_t *sub, cand_t *trans, int unset,
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

    int mins=9;
    
    barr_t mask_grp=0,        // mask grouped lattice
           mask_cnt=(1<<9)-1;

    for(int i=0; i<9; i++) {
        if(cand_isset(sub+i)) continue;
        int n=cand_num(sub+i);
        if(n<mins) mins=n;
    }

    for(int size=mins; size<unset; size++) {
        barr_t arr=(1<<size)-1,
               arr_max=arr<<(9-size);

        for(int i=0; i<9; i++) {
            if(cand_isset(sub+i)) continue;
            if(cand_num(sub+i)==size) mask_cnt&=~(1<<i);
        }

        for(;arr<=arr_max; arr=comb_next(arr, 9)) {
            if(arr&(mask_grp|mask_cnt)) continue;

            // union cands
            barr_t cnds=0;
            for(int i=0; i<9; i++) {
                if(arr&(1<<i)) cnds|=cand_bins(sub+i);
            }

            int num_u=bit_num_bin(cnds, 9);
            if(num_u>size) continue;
            if(num_u<size) return SCAN_ERROR;

            *sub_pos=0;
            for(int i=0; i<9; i++) {
                if(cand_isset(trans+i)) continue;

                if(!(cnds&(1<<i))) continue;

                // cross match in group
                barr_t arr_m=cand_bins(trans+i)&(~arr);
                if(!arr_m) continue;

                *sub_pos|=1<<i;
            }

            if(*sub_pos) {
                *sub_grp=arr;
                return SCAN_SUCC;
            }

            mask_grp|=arr;
        }
    }

    return SCAN_NONE;
}
