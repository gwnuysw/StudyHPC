#!/bin/csh

cc -c foo.c
cc -c prt.c
cc -o foo foo.o prt.o
foo
echo $?
