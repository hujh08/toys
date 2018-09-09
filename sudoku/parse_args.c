// parse arguments

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "sudoku.h"

void parse_args(int argc, char **argv, matrix *mat) {

   int opt;
   char *optstring="v";
   static struct option long_options[]={
       {"verbose",  no_argument, NULL, 'v'},
       {0, 0, 0, 0}
   };

   mat->verbose=0;
   opterr=0;
   while((opt = getopt_long(argc, argv,
                            optstring,
                            long_options,
                            NULL)) != -1)
   {
        switch(opt) {
            case 'v':
                mat->verbose=1;
                break;
            default:
                printf("unsupported option %c\n", opt);
                exit(-1);
        }
   }
}