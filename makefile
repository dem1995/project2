2.out: alloc_main.c file_io.c fit_algos.c memory.c block.c block_buddy_methods.c
	gcc alloc_main.c -o 2.out
all: 2.out
clean:
	$(RM) 2.out
