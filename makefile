2.out: allocationMain.c
	gcc allocationMain.c -o 2.out
all: 2.out
clean:
	$(RM) 2.out
