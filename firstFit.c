#include "memory.c"
#include <stdlib.h>

void firstFitProcess(memory* mem, int size, char* label);
bool bestFitProcess(memory* mem, int size, char* label);

void main()
{
	printf("Creating memory of size 21\n");
	memory mem = createMemory(21);
	printf("\n");

	printf("FirstFit adding size 13 fred\n");
	firstFitProcess(&mem, 13, "fred");	
	printMemContents(mem);
	printf("\n");

	printf("FirstFit adding size 6 geore\n");
	firstFitProcess(&mem, 6, "george");
	printMemContents(mem);
	printf("\n");
	
	printf("Releasing fred\n");
	releaseProcess(&mem, "fred");
	cleanMemory(mem);
	printMemContents(mem);
	printf("\n");

	printf("BestFit adding harold\n");
	bestFitProcess(&mem, 1, "harold");
	printMemContents(mem);
	printf("\n");

	printf("End");
	freeMemory(mem);
	return 0;
}


void firstFitProcess(memory* mem, int size, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size && !(b->isProcess))
		{
			printMemContents(*mem);
			spawnProcess(mem, b, label, size);
			printMemContents(*mem);
		}
	}
}

bool bestFitProcess(memory* mem, int size, char* label)
{
	int minAccomodatingBlockSize = -1;
	block* bestFitBlock = NULL;

	//Cycle through the blocks, finding the minimum accomodating block size
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size)
			if(!(b->isProcess))
				if (minAccomodatingBlockSize < 0 || b->size < minAccomodatingBlockSize)
				{
					minAccomodatingBlockSize = b->size;
					bestFitBlock = b;
				}		
	}

	//If there were no sufficiently-large blocks
	if (minAccomodatingBlockSize < 0)
		return false;
	else
	{
		spawnProcess(mem, bestFitBlock, label, size);
	}
	
}
