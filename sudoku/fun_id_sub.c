// functions to convert between global id and submat id

#include "sudoku.h"

// convert sub to global
int fid_row(int ofsub, int insub) {
    return insub+9*ofsub;
}

int fid_col(int ofsub, int insub) {
    return ofsub+9*insub;
}

int fid_blk(int ofsub, int insub) {
    return (3*(ofsub%3)+(insub%3))+9*(3*(ofsub/3)+(insub/3));
}

// convert global to sub
void fsubid_row(int n, int *no, int *ni) {
    *no=n/9;
    *ni=n%9;
}

void fsubid_col(int n, int *no, int *ni) {
    *no=n%9;
    *ni=n/9;
}

void fsubid_blk(int n, int *no, int *ni) {
    int r, c;
    fsubid_row(n, &r, &c);
    *no=NOBLK(r, c);
    *ni=NIBLK(r, c);
}