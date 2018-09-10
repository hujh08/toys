// basic operation of matrix

#include "sudoku.h"

// actually produce a empty matrix
void mat_init_unset(matrix *mat) {

    mat->unset=81;

    // init lattices
    for(int i=0; i<81; i++) lat_init_unset(mat->lat+i);

    // init sub-matrix
    for(int i=0; i<9; i++) {
        sub_init_unset(mat->rows+i);
        sub_init_unset(mat->cols+i);
        sub_init_unset(mat->blks+i);
    }
    return;
}

void mat_update(matrix *mat, int n, int d) {
    lattice *lat=mat->lat;

    lat_set(lat+n, d);
    mat->unset--;

    // update submatrix
    submat *rows=mat->rows,
           *cols=mat->cols,
           *blks=mat->blks;
    int r=n/9,
        c=n-9*r,
        b=NOBLK(r, c),
        s=NIBLK(r, c);

    // set down a number
    sub_set(rows+r, d);
    sub_set(cols+c, d);
    sub_set(blks+b, d);

    // del lattice in candidate of all numbers
    sub_del_lat(rows+r, c);
    sub_del_lat(cols+c, r);
    sub_del_lat(blks+b, s);

    // handle row and col
    for(int i=0; i<9; i++) {
        mat_del_latnum(mat, i+9*r, d);
        mat_del_latnum(mat, c+9*i, d);
    }

    // handle block
    int br=(r/3)*3,   // id of the block's start lat
        bc=(c/3)*3;
    for(int j=0; j<3; j++) {
        for(int i=0; i<3; i++) {
            mat_del_latnum(mat, bc+i+9*(br+j), d);
        }
    }
}

// del a number from candidate of a lattice
void mat_del_latnum(matrix *mat, int n, int d) {
    // if(lat_isset(mat->lat+n)) return;
    // if(!lat_iscand(mat->lat+n, d)) return;

    int r=n/9,
        c=n-9*r,
        b=NOBLK(r, c),
        s=NIBLK(r, c);

    lat_del_num(mat->lat+n, d);

    sub_del_latnum(mat->rows+r, c, d);
    sub_del_latnum(mat->cols+c, r, d);
    sub_del_latnum(mat->blks+b, s, d);
}