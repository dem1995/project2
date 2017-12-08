#include "memory.c"
#include <stdlib.h>

void firstFitProcess(memory* mem, int size);

void main()
{
	memory mem = createMemory(20);

	
	firstFitProcess(&mem, 13);

	printMemContents(mem);
	freeMemory(mem);
	return 0;
}


void firstFitProcess(memory* mem, int size)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size)
		{
			spawnProcess(mem, b, "", size);
		}
	}
}
