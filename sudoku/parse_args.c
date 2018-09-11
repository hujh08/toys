// parse arguments

#include <stdio.h>
#include <stdlib.h>
// #include <ctype.h>
// #include <string.h>
#include <getopt.h>

#include "sudoku.h"

void parse_args(int argc, char **argv, matrix *mat) {

   int opt;
   char *optstring="v::";
   static struct option long_options[]={
       {"verbose",  optional_argument, NULL, 'v'},
       {0, 0, 0, 0}
   };

   mat->verbose=PRINT_NONE;
   opterr=0;
   while((opt = getopt_long(argc, argv,
                            optstring,
                            long_options,
                            NULL)) != -1)
   {
        switch(opt) {
            case 'v':
                if(optarg) {
                    mat->verbose=atoi(optarg);
                    // char *s=optarg;
                    // int isnum=1;
                    // while(s) {
                    //   if(!isdigit(s)) {
                    //     isnum=0;
                    //     break;
                    //   }
                    //   s++;
                    // }
                    // if(isnum) mat->verbose=atoi(optarg);
                    // else {
                    //   if(strcmp(optarg, "debug")) mat->verbose=PRINT_DEBUG;
                    //   else if(strcmp(optarg, "info")) mat->verbose=PRINT_INFO;
                    // }
                }
                else mat->verbose=PRINT_INFO;
                break;
            default:
                printf("unsupported option %c\n", opt);
                exit(-1);
        }
   }
}