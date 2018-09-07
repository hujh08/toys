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
		printf("number of candidate: %i\n", lat->np);
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
		printf("        ");
		for(int i=0; i<9; i++, n++) {
			if(i==3 || i==6) printf("  ");
			if(lat[n].set) printf("%i ", lat[n].number);
			else printf("x ");
		}
		printf("\n");
	}
    printf("\n");
}

// initial an unset lattice
void lat_unset(lattice *lat) {
	lat->set=0;
	lat->np=9;             // all number is in candidate
	for(int i=0; i<9; i++) lat->cand[i]=1;
}

// determine whether lattice is set down
int lat_isset(lattice *lat) {
	return lat->set;
}

// determine whether number is in candidate of a lattice
int lat_iscand(lattice *lat, int d) {
	return lat->cand[d-1];
}

// return first candidate
int lat_1stcand(lattice *lat) {
	// if(lat->set) return -1;
	for(int n=0; n<9; n++) {
		if(lat->cand[n]) return n+1;
	}
	return 0;
}

// exclude a number for unset lattice
void lat_exclude(lattice *lat, int d) {
	int *ps=lat->cand;
	if(ps[d-1]==1) {
		ps[d-1]=0;
		lat->np--;
	}
}