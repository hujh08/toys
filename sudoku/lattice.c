// some functions for basic operation

#include <stdio.h>

#include "sudoku.h"

// print function
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
    printf("\n");
}

// initial an unset lattice
void lat_init_unset(lattice *lat) {
	lat->cnds.set=0;
	bit_fill(&(lat->cnds.arr), 9);
}

// set down a lattice
void lat_set(lattice *lat, int d) {
    lat->cnds.set=1;
    lat->number=d;
}

// whether lattice is set down
int lat_isset(lattice *lat) {
	return lat->cnds.set;
}

// whether number is in candidate of a lattice
int lat_iscand(lattice *lat, int d) {
	return bit_has(&(lat->cnds.arr), d-1);
}

// return first candidate
int lat_1stcand(lattice *lat) {
	// if(lat->set) return -1;
	return bit_1st(&(lat->cnds.arr))+1;
}

// number of candidate
int lat_cand_num(lattice *lat) {
	return cand_num(&(lat->cnds));
}

// exclude a number for unset lattice
void lat_del_num(lattice *lat, int d) {
	bit_unset(&(lat->cnds.arr), d-1);
}