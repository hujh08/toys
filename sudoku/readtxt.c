/*
    read txt to initiate matrix
    use 0 for unset
    the last empties can be omitted
*/

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

void readtxt(char *filename, matrix *mat) {
    char line[100];
    FILE *fp=fopen(filename, "r");
    int n=0, nl=0;

    while(fgets(line, 100, fp)) {
        nl++;
        for(int i=0; i<100; i++) {
            if(line[i]=='\0') break;
            if(line[i]=='\n') continue;
            if(n>=81) {
                printf("wrong format for input\n");
                fclose(fp);
                exit(-1);
            }
            int a=line[i]-'0';
            if(a<0 || a>9) {
                printf("%s:%i:%i: unexpected char: %c\n", filename, nl, i+1, line[i]);
                fclose(fp);
                exit(-1);
            }
            if(a!=0) mat_update(mat, n, a);
            n++;
        }
    }
}