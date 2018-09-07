// structures for sudoku

typedef struct
{
    int set,
        number,
        np,        // number of candidates
        cand[9];  // candidate number if unset
} lattice;

typedef struct
{
	int unset;  // number of unset
	lattice lat[81];
} matrix;

int nsol;  // number of solution

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
