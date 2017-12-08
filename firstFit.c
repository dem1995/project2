#include "memory.c"
#include <stdlib.h>

void firstFitProcess(memory* mem, int size, char* label);

void main()
{
	memory mem = createMemory(20);
	
	printMemContents(mem);
	printf("1\n");
	
	/*char* label = malloc(4 * sizeof(char));
	label = "fred";*/
	firstFitProcess(&mem, 13, "fred");
	
	printMemContents(mem);
	printf("2\n");

	firstFitProcess(&mem, 6, "george");
	
	printMemContents(mem);
	printf("3\n");

	releaseProcess(&mem, "fred");
	cleanMemory(mem);

	printMemContents(mem);

	printf("4\n");
	bestFitProcess(&mem, 1, "harold");
	printMemContents(mem);
	printf("4\n");

	freeMemory(mem);
	return 0;
}


void firstFitProcess(memory* mem, int size, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size && !(b->isProcess))
		{
			printf("writing over %s\n", b->label);
			printMemContents(*mem);
			printf("7\n");
			spawnProcess(mem, b, label, size);
			printMemContents(*mem);
			printf("7\n");
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
		spawnProcess(mem, bestFitBlock, label, size);
	}
	
}
