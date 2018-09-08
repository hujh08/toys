// functions for operatation of bit array

#include <stdio.h>

#include "bitarr.h"

void bit_clear(bitarr *bta) {
    bta->num=0;
    bta->bitarr=0;
}

void bit_fill(bitarr *bta, int len) {
    bta->num=len;
    bta->bitarr=(1<<len)-1;
}

// set/unset element
void bit_set(bitarr *bta, int n) {
    if(!bit_has(bta, n)) {
        bta->num++;
        bta->bitarr|=(1<<n);
    }
}

void bit_unset(bitarr *bta, int n) {
    if(bit_has(bta, n)) {
        bta->num--;
        bta->bitarr&=~(1<<n);
    }
}

// io functions
void bit_print(bitarr *bta) {
    int num=bit_num(bta);

    if(bit_isempty(bta)) {
        printf("%i elements: empty bit array\n", num);
        return;
    }

    printf("%i elements:", num);
    for(int i=0; i<MAXLEN; i++) {
        if(bit_has(bta, i)) printf(" %i", i);;
    }
    printf("\n");
}

// if contain a element
int bit_has(bitarr *bta, int n) {
    return (bta->bitarr)&(1<<n);
}

int bit_isempty(bitarr *bta) {
    return bta->bitarr==0;
}

// number of elements in array
int bit_num(bitarr *bta) {
    return bta->num;
}

// first element
int bit_1st(bitarr *bta) {
    if(bit_isempty(bta)) return -1;
    for(int i=0; i<MAXLEN; i++) {
        if(bit_has(bta, i)) return i;
    }
    return -1;
}