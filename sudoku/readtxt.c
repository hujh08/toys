/*
    read txt to initiate matrix
    use 0 for unset
*/

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

void readtxt(char *filename, matrix *mat) {
    lattice *lat=mat->lat;
    mat->unset=81;

    char line[12];
    FILE *fp=fopen(filename, "r");
    int n=0;

    while(fgets(line, 12, fp)) {
        if(line[9]!='\n' && line[9]!='\0') {
            printf("wrong format for input\n");
            exit(-1);
        }

        for(int i=0; i<9; i++) {
            if(line[i]!='0') {
            	lat[n].set=1;
            	lat[n].number=line[i]-'0';
                mat->unset--;
            } else lat_unset(lat+n);
            n++;
        }
    }

    fclose(fp);
}