GCC = gcc

all: main.o
	$(GCC) main.o

main.o: main.c main.h
	$(GCC) -c main.c

clean:
	rm *.o
	rm *~

run: all
	./a.out

