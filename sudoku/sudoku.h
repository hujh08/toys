// structures for sudoku

#include "bitarr.h"

// structure for lattice
typedef struct
{
    int set,
        number;
    bitarr cand;  // use bit array to store number candidates
} lattice;

// store for candidate lattices for a number
typedef struct
{
	int set;
	bitarr cand;
} number_t;

// structure for sub-matrix, i.e. row, column, block
typedef struct
{
	int unset;    // number of unset
	number_t nums[9];
} submat;

typedef struct
{
	int ntry;   // hight of try
	int unset;  // number of unset

	int verbose;  // whether print the step

	lattice lat[81];

	submat rows[9], cols[9], blks[9];
} matrix;

int nsol;  // number of solution
int ntry;  // number of try

// macros
// for block
#define NOBLK(r, c) ((c/3)+3*(r/3))  // id of block
#define NIBLK(r, c) ((c%3)+3*(r%3))  // id in block
// for scan result
#define SCAN_ERROR -1
#define SCAN_NONE  0
#define SCAN_SUCC  1

// functions for io
extern void readtxt(char *, matrix *);
extern void print_mat(matrix *);
extern void print_lat(lattice *);
extern void print_sub(submat *);

// functions for single lattice
extern void lat_unset(lattice *);
extern int lat_isset(lattice *);
extern int lat_iscand(lattice *, int );
extern int lat_1stcand(lattice *);
extern void lat_exclude(lattice *, int );

// functions for number structure
extern void num_init(number_t *);
extern int num_isset(number_t *);
extern int num_1stcand(number_t *);

// functions for sub-matrix
extern void sub_init(submat *);
extern void sub_set(submat *, int );
extern void sub_add(submat *, int , int );
extern void sub_del_latnum(submat *, int , int );
extern void sub_del_lat(submat *, int );
extern int sub_isset(submat *);

// functions used to fill lattices
extern int mat_analysis(matrix *);
extern void mat_update(matrix *, int , int );
extern void mat_fill_try(matrix *);
extern int mat_fill_notry(matrix *);

// functions to scan matrix
extern int scan_lats(matrix *, int *, int *);
extern int scan_rows(matrix *, int *, int *);
extern int scan_cols(matrix *, int *, int *);
extern int scan_blks(matrix *, int *, int *);
