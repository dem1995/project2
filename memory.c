#include <stdio.h>
#include <stdlib.h>
#include "block.c"


typedef struct memory
{
	int totalMemory;
	int freeMemory;
	
	block* firstBlock;
}memory;

memory createMemory(int size)
{
	//This is some janky initialization syntax
	memory newMemory;
	newMemory.totalMemory = size;
	newMemory.freeMemory = size;
	newMemory.firstBlock = malloc(sizeof(block));
	*(newMemory.firstBlock) = createEmptyBlock(size, NULL, NULL);
	/*{
		.totalMemory = size,
		.freeMemory = size,
		.firstBlock = createEmptyBlock(size, NULL, NULL)
	};*/

	return newMemory;
}

void cleanMemory(memory mem)
{
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		//If we're looking at a free block of memory, and not a process block, and the next block isn't null
		if (!(b->isProcess)&&!(b->nextBlock==NULL))
		{
			if (!(b->nextBlock->isProcess))
			{
				//Combine the blocks
				block* thisBlock = b;
				block* nextBlock = b->nextBlock;
				thisBlock->size += nextBlock->size;
				thisBlock->nextBlock = nextBlock->nextBlock;
			}
		}
	}
}

void spawnProcess(memory* mem, block* theBlock, char* label, int processSize)
{
	if (processSize > theBlock->size)
		printf("Process %s size too big", label);
	else if (processSize == theBlock->size)
		*(mem->firstBlock) = createProcess(processSize, label, NULL, theBlock->nextBlock);
	else
	{
		{
			if (theBlock->nextBlock == NULL)
				theBlock->nextBlock = malloc(sizeof(block));
			*(theBlock->nextBlock) = createProcess(processSize, label, theBlock, theBlock->nextBlock);
			theBlock->size -= processSize;
		}
	}
}

void printMemContents(memory mem)
{
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		printf("address of b is %i", b);
		printf("address of b->nextBlock is %i", b);
		printBlockContents(*b);
	}
}






