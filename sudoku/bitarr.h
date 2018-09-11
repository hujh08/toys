// bit array

typedef int barr_t;

#define MAXLEN (8*sizeof(barr_t))

typedef struct
{
	int num;      // number of elements in array
	barr_t bitarr;
} bitarr;


// functions for bit array
// set bit array
extern void bit_clear(bitarr *);
extern void bit_fill(bitarr *, int );
extern void bit_set(bitarr *, int );
extern void bit_unset(bitarr *, int );

// io functions
extern void bit_print(bitarr *);
extern void bit_print_bin(barr_t , int );
extern void bit_print_arrbin(bitarr , int );

// functions to determine something
extern int bit_has(bitarr *, int );
extern int bit_isempty(bitarr *);

// info of array
extern int bit_num(bitarr *);
extern int bit_num_bin(barr_t , int );
extern barr_t bit_bins(bitarr *);

// return an element
extern int bit_1st(bitarr *);