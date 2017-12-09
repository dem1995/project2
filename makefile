2.out: alloc_main.o
	gcc alloc_main.o -o 2.out

alloc_main.o: alloc_main.c file_io.o fit_algos.o

file_io.o: file_io.c

fit_algos.o: fit_algos.c memory.c block.c

all: 2.out
clean:
	$(RM) 2.out
