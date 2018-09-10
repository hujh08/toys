// functions for sub-matrix

#include "sudoku.h"

void sub_init_unset(submat *sub) {
    sub->unset=9;
    for(int i=0; i<9; i++) cand_init_unset(sub->nums+i);
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
    cand_set(sub->nums+d-1, li);
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

// functions for all kinds of cand structure
void cand_init_unset(cand_t *cnd) {
    cnd->set=0;
    bit_fill(&(cnd->arr), 9);
}
void cand_init_unset_empty(cand_t *cnd) {
    cnd->set=0;
    bit_clear(&(cnd->arr));
}

// add a binary bit
void cand_set(cand_t *cnd, int n) {
    bit_set(&(cnd->arr), n);
}

int cand_1stcand(cand_t *cnd) {
    return bit_1st(&(cnd->arr));
}

int cand_isset(cand_t *cnd) {
    return cnd->set;
}

int cand_num(cand_t *cnd) {
    return bit_num(&(cnd->arr));
}

barr_t cand_bins(cand_t *cnd) {
    return bit_bins(&(cnd->arr));
}