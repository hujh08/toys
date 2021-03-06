// fill matrix

#include "sudoku.h"

// initial analysis must be done before
int mat_fill_notry(matrix *mat) {
    int n, d, found;

    while(mat->unset) {
        // print_lat(mat->lat);
        found=scan_rows(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            mat_update(mat, n, d);
            (mat->nstep)++;
            print_update(mat, n, "rows");
            continue;
        }

        found=scan_cols(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            mat_update(mat, n, d);
            (mat->nstep)++;
            print_update(mat, n, "cols");
            continue;
        }

        found=scan_blks(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            mat_update(mat, n, d);
            (mat->nstep)++;
            print_update(mat, n, "blks");
            continue;
        }
        
        found=scan_lats(mat, &n, &d);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {
            mat_update(mat, n, d);
            (mat->nstep)++;
            print_update(mat, n, "lats");
            continue;
        }

        // strategies to reduce candidates
        found=mat_cross(mat);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {(mat->nstep)++; continue;}

        found=mat_group_sub(mat);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {(mat->nstep)++; continue;}

        found=mat_group_num(mat);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {(mat->nstep)++; continue;}
        /*
            comment about group_num:
                mat_cross can be seen as a special mat_group_num
                    which is done in col/row-blk plane and group size=1
        */

        found=mat_chain(mat);
        if(found==SCAN_ERROR) return found;
        if(found==SCAN_SUCC) {(mat->nstep)++; continue;}

        if(found==SCAN_NONE) return found;
    }

    print_result(mat);

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
        if(lat_iscand(lat+n, i)) {
            ntry++;

            matrix mat0=*mat;
            mat0.ntry++;

            mat_update(&mat0, n, i);

            conflict=mat_fill_notry(&mat0);

            if(conflict==0) mat_fill_try(&mat0);
        }
    }
}