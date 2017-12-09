2.out: alloc_main.c
	gcc alloc_main.c -o 2.out
all: 2.out
clean:
	$(RM) 2.out
