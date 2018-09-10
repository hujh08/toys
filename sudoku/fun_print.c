// print function

#include <stdlib.h>
#include <stdio.h>

#include "sudoku.h"

// print lattice
void print_lat(lattice *lat) {
    if(lat_isset(lat)) {
        printf("set\n");
        printf("number: %i\n", lat->number);
    } else {
        printf("unset\n");
        printf("number of possible: %i\n", lat_cand_num(lat));
        printf("candidate:");
        for(int i=1; i<=9; i++) {
            if(lat_iscand(lat, i)) printf(" %i", i);
        }
        printf("\n");
    }
}

// print sub-matrix
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

// print matrix
void print_mat(matrix *mat) {
    lattice *lat=mat->lat;
    int n=0;
    for(int j=0; j<9; j++) {
        if(j==3 || j==6) printf("\n");
        printf("       ");
        for(int i=0; i<9; i++, n++) {
            if(i==3 || i==6) printf("  ");
            if(lat_isset(lat+n)) printf(" %i", lat[n].number);
            else printf(" x");
        }
        printf("\n");
    }
}

// print lattice update
void print_update(matrix *mat, int n, char *scan) {
    if(!mat->verbose) return;
    int r=n/9,
        c=n-9*r,
        d=lat_num(mat->lat+n);
    printf("scan %s found: lat (%i, %i), num %i\n",
                scan, r+1, c+1, d);
    printf("\n");
    print_mat(mat);
    printf("\n");
}

// print cross analysis
void print_cross(matrix *mat, int cross, int s0, int s1, int d) {
    if(!mat->verbose) return;

    char *sub0, *sub1;
    switch(cross) {
        case CROSS_BLK_ROW:
            sub0="blk"; sub1="row"; break;
        case CROSS_BLK_COL:
            sub0="blk"; sub1="col"; break;
        case CROSS_ROW_BLK:
            sub0="row"; sub1="blk"; break;
        case CROSS_COL_BLK:
            sub0="col"; sub1="blk"; break;
        default:
            printf("error for cross print\n");
            exit(-1);
    }

    printf("cross: %s %i, %s %i, num %i\n",
                sub0, s0+1, sub1, s1+1, d);
}

void print_group(matrix *mat, int b, 
                 barr_t val, barr_t pos, int group) {
    if(!mat->verbose) return;

    char *typeg, *typep, *typev;
    barr_t tmp;
    switch(group) {
        case MARK_ROW:
            typeg="row"; typep="col"; typev="num"; break;
        case MARK_COL:
            typeg="col"; typep="row"; typev="num"; break;
        case MARK_BLK:
            typeg="blk"; typep="lat"; typev="num"; break;
        case GRUP_NUM:
            typeg="num";
            tmp=pos; pos=val; val=tmp;
            typep="row"; typev="col";
            break;
        default:
            printf("error for cross print\n");
            exit(-1);
    }

    printf("group: %s %i, %s ", typeg, b+1, typep);
    for(int i=0; i<9; i++) {
        if(pos&(1<<i)) printf("%i", i+1);
    }
    printf(", %s ", typev);
    for(int i=0; i<9; i++) {
        if(val&(1<<i)) printf("%i", i+1);
    }
    printf("\n");
}

// print finished matrix
void print_result(matrix *mat) {
    printf("solution %i:\n", ++nsol);
    printf("total try: %i\n", ntry);
    printf("level of try: %i\n", mat->ntry);
    print_mat(mat);
    printf("\n");
}