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

bool bestFitProcess(memory* mem, int size)
{
	int minAccomodatingBlockSize = -1;
	block* bestFitBlock = NULL;

	//Cycle through the blocks, finding the minimum accomodating block size
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size > size)
		{
			if (minAccomodatingBlockSize < 0 || b->size < minAccomodatingBlockSize)
			{
				minAccomodatingBlockSize = b->size;
				bestFitBlock = b;
			}
		}		
	}

	//If there were no sufficiently-large blocks
	if (minAccomodatingBlockSize < 0)
		return false;
	else
	{
		spawnProcess(mem, bestFitBlock, "", size);
	}
	
}
