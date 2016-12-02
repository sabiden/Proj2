GCC = gcc

all: main.o
	$(GCC) main.o -lreadline -o SHELL 

main.o: main.c main.h
	$(GCC) -c main.c

clean:
	rm *.o
	rm *~

run: all
	./SHELL

