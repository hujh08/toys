// fill matrix

#include <stdio.h>

#include "sudoku.h"

// initial analysis must be done before
int mat_fill_notry(matrix *mat) {
    int n, d, found;

    int verbose=mat->verbose;

    while(mat->unset) {
        found=scan_lats(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            if(verbose) {
                print_mat(mat);
                printf("lats scan found: lat %i, num %i\n", n+1, d);
                printf("\n");
            }
            mat_update(mat, n, d);
            continue;
        }

        // printf("begin to scan rows\n");
        found=scan_rows(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            if(verbose) {
                print_mat(mat);
                printf("rows scan found: lat %i, num %i\n", n+1, d);
                printf("\n");
            }
            mat_update(mat, n, d);
            continue;
        }

        // printf("begin to scan cols\n");
        found=scan_cols(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            if(verbose) {
                print_mat(mat);
                printf("cols scan found: lat %i, num %i\n", n+1, d);
                printf("\n");
            }
            mat_update(mat, n, d);
            continue;
        }

        // printf("begin to scan blks\n");
        found=scan_blks(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            if(verbose) {
                print_mat(mat);
                printf("blks scan found: lat %i, num %i\n", n+1, d);
                printf("\n");
            }
            mat_update(mat, n, d);
            continue;
        }

        found=mat_group(mat);
        if(found==SCAN_ERROR) return found;

        if(found==SCAN_NONE) return found;
    }

    printf("solution %i:\n", ++nsol);
    printf("total try: %i\n", ntry);
    printf("level of try: %i\n", mat->ntry);
    print_mat(mat);

    return SCAN_SUCC;
}

// try candidates for unset lattice
void mat_fill_try(matrix *mat) {
    lattice *lat=mat->lat;

    int n, conflict;

    for(n=0; n<81; n++) {
        if(!lat_isset(lat+n)) break;
    }

    for(int i=1; i<=9; i++) {
        // if(lat[n].cand[i]) {
        if(lat_iscand(lat+n, i)) {
            ntry++;

            matrix mat0=*mat;
            mat0.ntry++;

            // printf("try %i for lattice %i\n", i+1, n+1);
            mat_update(&mat0, n, i);

            conflict=mat_fill_notry(&mat0);

            if(conflict==0) mat_fill_try(&mat0);
        }
    }
}