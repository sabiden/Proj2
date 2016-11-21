GCC = gcc

all: proj.o
	$(GCC) proj.o

proj.o.o: proj.c proj.h
	$(GCC) -c proj.c

clean:
	rm *.o
	rm *~

run: all
	./a.out

