2.out: firstFit.c
	gcc firstFit.c -o 2.out
all: 2.out
clean:
	$(RM) 2.out
