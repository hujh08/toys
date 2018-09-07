// analysis matrix

// #include <stdio.h>

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
			}
		}
	}

    return 0;
}

void mat_update(matrix *mat, int n, int d) {
    lattice *lat=mat->lat;

    lat[n].set=1;
    lat[n].number=d;

    mat->unset--;

    lat_update(lat, n);
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