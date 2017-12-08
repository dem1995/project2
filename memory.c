#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.c"


typedef struct memory
{
	int totalMemory;
	int freeMemory;
	
	block* firstBlock;
}memory;

void printMemContents(memory mem);

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

void freeMemory(memory mem)
{
	for (block* b = mem.firstBlock; b != NULL;)
	{

		block* nextBlockPtrCopy = b->nextBlock;
		
		if (b != NULL)
		{
			//if (b->label != NULL)
				//free(b->label);
			free(b);
		}

		b = nextBlockPtrCopy;
	}
}

void spawnProcess(memory* mem, block* theBlock, char* label, int processSize)
{
	if (processSize > theBlock->size)
		printf("Process %s size too big", label);
	else if (processSize == theBlock->size)
		*(mem->firstBlock) = createProcess(processSize, label, theBlock->prevBlock, theBlock->nextBlock);
	else
	{
		if (theBlock->prevBlock == NULL)
		{
			// say we have block t. a->t->z becomes a->newblock->t->z
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, NULL, theBlock);
			mem->firstBlock = theBlock->prevBlock;
			theBlock->size -= processSize;

		}
		else
		{
			printf("8\n");
			printBlockContents(*theBlock);
			// say we have block t. a->t->z becomes a->newblock->t->z
			block* prevBlockPointerCopy = theBlock->prevBlock;
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, prevBlockPointerCopy, theBlock);

			//set the nextBlock of the block 2 before this one to this one
			prevBlockPointerCopy->nextBlock = (theBlock->prevBlock);

			theBlock->size -= processSize;
			printBlockContents(*theBlock);
			printf("8\n");
		}
		

	}
}

void releaseProcess(memory* mem, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->isProcess)
			if (strcmp(b->label, label) == 0)
			{
				printf("removing: %s\n", b->label);
				printf("before: ");
				printMemContents(*mem);
				releaseBlock(b);
				printf("after: ");
				printMemContents(*mem);
			}
	}
}

void printMemContents(memory mem)
{
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		printBlockContents(*b);
	}
}






