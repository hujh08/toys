// fill matrix

#include <stdio.h>

#include "sudoku.h"

// initial analysis must be done before
int mat_fill_notry(matrix *mat) {
    int toset, d;
    lattice *lat=mat->lat;
    while(mat->unset) {
        for(toset=0; toset<81; toset++) {
            if(lat_isset(lat+toset)) continue;
            if(lat[toset].np==0) return -1;
            if(lat[toset].np==1) break;
        }

        if(toset==81) return 0;

        d=lat_1stcand(lat+toset);

        // printf("update lattice %i with %i\n", toset+1, d);
        mat_update(mat, toset, d);
    }

    printf("solution %i:\n", ++nsol);
    printf("total try: %i\n", ntry);
    printf("level of try: %i\n", mat->ntry);
    print_mat(mat);

    return 1;
}

// try candidates for unset lattice
void mat_fill_try(matrix *mat) {
    lattice *lat=mat->lat;

    int n, conflict;

    for(n=0; n<81; n++) {
        if(!lat_isset(lat+n)) break;
    }

    for(int i=0; i<9; i++) {
        if(lat[n].cand[i]) {
            ntry++;

            matrix mat0=*mat;
            mat0.ntry++;

            // printf("try %i for lattice %i\n", i+1, n+1);
            mat_update(&mat0, n, i+1);

            conflict=mat_fill_notry(&mat0);

            if(conflict==0) mat_fill_try(&mat0);
        }
    }
}