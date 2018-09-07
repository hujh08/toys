#!/bin/bash

name=$(basename `pwd`)
mkf=makefile
echo "NAME = $name

# the C compiler
CC = gcc
# options for the C compiler
COPTS = -std=c99 -O2 -Wall # -pedantic" >$mkf

objects=`ls | grep -v $name | sed -n 's/\.c$/.o/gp' |
         awk '{if(NR!=1) print "          "$0"\\\";
               else print $0"\\\";}'`
objects=${objects%\\}

echo "
objects = $objects
">>$mkf

echo '$(NAME) : $(objects)
	$(CC) -o $(NAME) $^ $(LIBS)

%.o : $(STDH) %.c
	$(CC) $(COPTS) $(INCLUDES) -c $^ -o $@

clean :
	rm -f $(objects)'>>$mkf
