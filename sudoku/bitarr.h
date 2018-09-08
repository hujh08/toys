// bit array

typedef int tbarr;

#define MAXLEN (8*sizeof(tbarr))

typedef struct
{
	int num;      // number of elements in array
	tbarr bitarr;
} bitarr;


// functions for bit array
// set bit array
extern void bit_clear(bitarr *);
extern void bit_fill(bitarr *, int );
extern void bit_set(bitarr *, int );
extern void bit_unset(bitarr *, int );

// io functions
extern void bit_print(bitarr *);

// functions to determine something
extern int bit_has(bitarr *, int );
extern int bit_isempty(bitarr *);

// info of array
extern int bit_num(bitarr *);

// return an element
extern int bit_1st(bitarr *);