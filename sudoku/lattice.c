// some functions for basic operation

#include "sudoku.h"

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

// whether lattice is set to num
int lat_isnum(lattice *lat, int d) {
	return lat->cnds.set && lat->number==d;
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

// return number of lat
int lat_num(lattice *lat) {
	return lat->number;
}

// number of candidate
int lat_cand_num(lattice *lat) {
	return cand_num(&(lat->cnds));
}

// exclude a number for unset lattice
void lat_del_num(lattice *lat, int d) {
	bit_unset(&(lat->cnds.arr), d-1);
}