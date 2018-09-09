// functions for sub-matrix

#include <stdio.h>

#include "sudoku.h"


void sub_init(submat *sub) {
    sub->unset=9;
    for(int i=0; i<9; i++) num_init(sub->nums+i);
}

// set down a number
void sub_set(submat *sub, int d) {
    if(!(sub->nums[d-1].set)) {
        sub->unset--;
        sub->nums[d-1].set=1;
    }
}

// add a candidate lattice for a number
void sub_add(submat *sub, int li, int d) {
    bit_set(&(sub->nums[d-1].arr), li);
}

// exclude a candidate number in a lattice
void sub_del_latnum(submat *sub, int li, int d) {
    bit_unset(&(sub->nums[d-1].arr), li);
}

// exclude a lattice for all numbers
void sub_del_lat(submat *sub, int li) {
    for(int d=0; d<9; d++)
        bit_unset(&(sub->nums[d].arr), li);
}

// whether lattice is set down
int sub_isset(submat *sub) {
    return sub->unset==0;
}

// io functions
void print_sub(submat *sub) {
    if(sub->unset==0) {
        printf("sub-matrix is set down\n");
        return;
    }

    number_t *nums=sub->nums;

    printf("%i set numbers:", 9-sub->unset);
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) printf(" %i", i+1);
    }
    printf("\n");
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) continue;
        printf("number %i: %i lat", i+1, cand_num(nums+i));
        if(!cand_isset(nums+i)) {
            for(int n=0; n<9; n++) {
                if(bit_has(&(nums[i].arr), n))
                    printf(" %i", n+1);
            }
            printf("\n");
        }
    }
}

// function about number
void num_init(number_t *num) {
    num->set=0;
    bit_clear(&(num->arr));
}

int num_1stcand(number_t *num) {
    return bit_1st(&(num->arr));
}

// functions for all kinds of cand structure
int cand_isset(cand_t *cnd) {
    return cnd->set;
}

int cand_num(cand_t *cnd) {
    return bit_num(&(cnd->arr));
}

barr_t cand_bins(cand_t *cnd) {
    return bit_bins(&(cnd->arr));
}