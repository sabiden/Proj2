GCC = gcc

all: main.o
	$(GCC) main.o  -o SHELL

main.o: main.c main.h
	$(GCC) -c main.c

clean:
	rm *.o
	rm *~

run: all
	./SHELL

