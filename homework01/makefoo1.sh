#!/bin/csh

cc -c foo.c
cc -c prt.c
ar -cr libport.a prt.o
cc -o foo foo.o -lprt -L.
foo
echo $?
