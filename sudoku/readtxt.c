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

// void readtxt(char *filename, matrix *mat) {
//     char line[12];
//     FILE *fp=fopen(filename, "r");
//     int n=0;

//     while(fgets(line, 12, fp)) {
//         if(line[9]!='\n' && line[9]!='\0') {
//             printf("wrong format for input\n");
//             exit(-1);
//         }

//         for(int i=0; i<9; i++) {
//             if(line[i]!='0') {
//                 mat_update(mat, n, line[i]-'0');
//             	// lat[n].set=1;
//             	// lat[n].number=line[i]-'0';
//                 // mat->unset--;
//             } //else lat_init_unset(lat+n);
//             n++;
//         }
//     }

//     fclose(fp);
// }