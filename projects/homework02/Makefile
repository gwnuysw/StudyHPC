area: area.o millisecond.o
	cc -o area area.o millisecond.o -lm
area.o: area.c millisecond.h
	cc -c area.c
millisecond.o: millisecond.c
	cc -c millisecond.c
clean:
	rm -f *.o
run:
	@area;echo $?
