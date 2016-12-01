GCC = gcc

all: main.o
	$(GCC) main.o  -lreadline -Wall -std=c99 -o SHELL 

main.o: main.c main.h
	$(GCC) -c main.c

clean:
	rm *.o
	rm *~

run: all
	./SHELL

