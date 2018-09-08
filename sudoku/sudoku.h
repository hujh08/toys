// structures for sudoku

#include "bitarr.h"

typedef struct
{
    int set,
        number;
    bitarr cand;  // use bit array to store number candidates
} lattice;

typedef struct
{
	int ntry;   // hight of try
	int unset;  // number of unset
	lattice lat[81];
} matrix;

int nsol;  // number of solution
int ntry;  // number of try

// functions for io
extern void readtxt(char *, matrix *);
extern void print_mat(matrix *);
extern void print_lat(lattice *);

// functions for single lattice
extern void lat_unset(lattice *);
extern int lat_isset(lattice *);
extern int lat_iscand(lattice *, int );
extern int lat_1stcand(lattice *);
extern void lat_exclude(lattice *, int );

// functions used to fill lattices
extern int mat_analysis(matrix *);
extern void mat_update(matrix *, int , int );
extern void mat_fill_try(matrix *);
extern int mat_fill_notry(matrix *);
