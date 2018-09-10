// structures for sudoku

#include "bitarr.h"

// structure for candidate, for lattice or number in row/col/block
typedef struct
{
	int set;
	bitarr arr;
} cand_t;

// structure for lattice
typedef struct
{
    // int set,
    //     number;
    // bitarr cand;  // use bit array to store number candidates

    int number;
    cand_t cnds;
} lattice;

// store for candidate lattices for a number
// typedef struct
// {
// 	int set;
// 	bitarr cand;
// } number_t;
typedef cand_t number_t;

// structure for sub-matrix, i.e. row, column, block
typedef struct
{
	int unset;    // number of unset
	cand_t nums[9];
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

// function to calculate id
typedef int (*fun_id)(int , int );
extern int fid_row(int , int );
extern int fid_col(int , int );
extern int fid_blk(int , int );

// functions for io
extern void readtxt(char *, matrix *);
extern void print_mat(matrix *);
extern void print_lat(lattice *);
extern void print_sub(submat *);

// function for arguments
extern void parse_args(int , char **, matrix *);

// functions for single lattice
extern void lat_init_unset(lattice *);
extern void lat_set(lattice *, int );
extern int lat_isset(lattice *);
extern int lat_iscand(lattice *, int );
extern int lat_1stcand(lattice *);
extern int lat_cand_num(lattice *);
extern void lat_del_num(lattice *, int );

// functions for candidate structure
extern int cand_1stcand(number_t *);
extern void cand_init_unset(number_t *);
extern int cand_isset(cand_t *);
extern int cand_num(cand_t *);
extern barr_t cand_bins(cand_t *);

// functions for sub-matrix
extern void sub_init_unset(submat *);
extern void sub_set(submat *, int );
extern void sub_add(submat *, int , int );
extern void sub_del_latnum(submat *, int , int );
extern void sub_del_lat(submat *, int );
extern int sub_isset(submat *);

// basic functions for matrix
extern void mat_init_unset(matrix *);
extern void mat_update(matrix *, int , int );
extern void mat_del_latnum(matrix *, int , int );

// functions to fill matrix
extern void mat_fill_try(matrix *);
extern int mat_fill_notry(matrix *);

// functions to scan matrix
extern int scan_lats(matrix *, int *, int *);
extern int scan_rows(matrix *, int *, int *);
extern int scan_cols(matrix *, int *, int *);
extern int scan_blks(matrix *, int *, int *);

// strategies to decrease candidates of lattices
extern int gen_group(cand_t *, cand_t *, int , int ,
                     barr_t *, barr_t *);
extern int mat_group_numsub(matrix *);
extern int mat_group_rowcol(matrix *);
extern int mat_cross(matrix *);
