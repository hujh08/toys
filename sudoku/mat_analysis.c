// analysis matrix

#include <stdio.h>

#include "sudoku.h"

// exclude possible number in row, column and block
void lat_update(lattice *, int );
void exclude_row(lattice *, int , int );
void exclude_col(lattice *, int , int );
void exclude_blk(lattice *, int , int );

int mat_analysis(matrix *mat) {
	lattice *lat=mat->lat;

	int n=0;
	for(int j=0; j<9; j++) {
		for(int i=0; i<9; i++, n++) {
			if(lat_isset(lat+n)) {
                lat_update(lat, n);
                // print_lat(lat+1);
                // printf("num: %i\n", num);
			}
		}
	}

    // analysis sub-matrix
    submat *rows=mat->rows,
           *cols=mat->cols,
           *blks=mat->blks;
    for(int i=0; i<9; i++) {
        sub_init(rows+i);
        sub_init(cols+i);
        sub_init(blks+i);
    }

    for(int r=0; r<9; r++) {
        for(int c=0; c<9; c++) {
            int n=c+9*r,
                b=NOBLK(r, c);

            if(lat_isset(lat+n)) {
                sub_set(rows+r, lat[n].number);
                sub_set(cols+c, lat[n].number);
                sub_set(blks+b, lat[n].number);
            } else {
                for(int i=1; i<=9; i++) {
                    if(!lat_iscand(lat+n, i)) continue;

                    sub_add(rows+r, c, i);
                    sub_add(cols+c, r, i);

                    sub_add(blks+b, NIBLK(r, c), i);
                }
            }

        }
    }

    return 0;
}

void mat_update(matrix *mat, int n, int d) {
    lattice *lat=mat->lat;

    // printf("update: lat %i, num %i\n", n+1, d);
    // print_mat(mat);

    lat_set(lat+n, d);

    mat->unset--;

    lat_update(lat, n);

    // update submatrix
    submat *rows=mat->rows,
           *cols=mat->cols,
           *blks=mat->blks;
    int r=n/9,
        c=n-9*r,
        b=NOBLK(r, c),
        s=NIBLK(r, c);

    // printf("rcbs: %i %i %i %i\n", r, c, b, s);

    // set down a number
    sub_set(rows+r, d);
    sub_set(cols+c, d);
    sub_set(blks+b, d);

    // del lattice in candidate of all numbers
    sub_del_lat(rows+r, c);
    sub_del_lat(cols+c, r);
    sub_del_lat(blks+b, s);

    // print_sub(mat->blks);

    // scan row and col
    for(int i=0; i<9; i++) {
        // handle rows and blocks touching the column
        sub_del_latnum(rows+i, c, d);
        sub_del_latnum(blks+NOBLK(i, c), NIBLK(i, c), d);

        // handle columns and blocks touching the row
        sub_del_latnum(cols+i, r, d);
        sub_del_latnum(blks+NOBLK(r, i), NIBLK(r, i), d);

        // printf("handle %i rows/cols\n", i);
        // print_sub(mat->blks);

        // printf("handle blks: %i %i; %i %i\n", 
        //             NOBLK(i, c), NIBLK(i, c),
        //             NOBLK(r, i), NIBLK(r, i));
        // print_sub(mat->blks);
    }

    // printf("after handle sub touching row/col\n");
    // print_sub(mat->rows+2);

    // handle rows and cols touching the block
    int br=(r/3)*3,   // id of the block's start lat
        bc=(c/3)*3;
    // printf("brc: %i %i\n", br, bc);
    for(int j=0; j<3; j++) {
        for(int i=0; i<3; i++) {
            sub_del_latnum(rows+br+j, bc+i, d);
            sub_del_latnum(cols+bc+i, br+j, d);
        }
    }



    // print_sub(mat->blks);
    // print_sub(mat->cols);
    // print_sub(mat->rows+2);
}

void lat_update(lattice *lat, int n) {
    int r=n/9,
        c=n-9*r,
        br=(r/3)*3,
        bc=(c/3)*3,
        b=bc+9*br;

    exclude_row(lat, r, lat[n].number);
    exclude_col(lat, c, lat[n].number);
    exclude_blk(lat, b, lat[n].number);
}

void exclude_row(lattice *lat, int r, int d) {
    // printf("exclude row: %i with d=%i\n", r, d);
    for(int i=0; i<9; i++) lat_exclude(lat+9*r+i, d);
}

void exclude_col(lattice *lat, int c, int d) {
    // printf("exclude col: %i with d=%i\n", c, d);
    for(int i=0; i<9; i++) lat_exclude(lat+c+9*i, d);
}

void exclude_blk(lattice *lat, int b, int d) {
    // printf("exclude block: %i with d=%i\n", b, d);
    for(int j=0; j<3; j++, b+=6) {
        for(int i=0; i<3; i++, b++) lat_exclude(lat+b, d);
    }
}