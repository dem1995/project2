#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.c"

#ifndef MEMORY_C
#define MEMORY_C

typedef struct memory
{
	unsigned long totalMemory;	
	block* firstBlock;
}memory;

void printAllMemContents(memory mem);

memory createMemory(unsigned long size)
{
	memory newMemory = {
		//.totalMemory = size,
		size,
		//.firstBlock = malloc(sizeof(block))
		malloc(sizeof(block))
	};

	*(newMemory.firstBlock) = createEmptyBlock(size, NULL, NULL);

	return newMemory;
}

void cleanMemory(memory mem)
{
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		//If we're looking at a free block of memory, and not a process block, and the next block isn't null
		if (!(b->isProcess)&&(b->nextBlock!=NULL))
		{
			if (!(b->nextBlock->isProcess))
			{
				//Combine the blocks
				block* thisBlock = b;
				block* nextBlock = b->nextBlock;
				thisBlock->size += nextBlock->size;
				thisBlock->nextBlock = nextBlock->nextBlock;
				free(nextBlock);
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
			if (b->label != NULL)
				free(b->label);
			free(b);
		}

		b = nextBlockPtrCopy;
	}
}

block* spawnProcess(memory* mem, block* theBlock, char* label, unsigned long processSize)
{
	if (processSize > theBlock->size)
	{
		printf("Process %s size too big", label);
		return NULL;
	}
	else if (processSize == theBlock->size)
	{

		//printf("before %s replacement: ", label);
		//printAllMemContents(*mem);
		*(theBlock) = createProcess(processSize, label, theBlock->prevBlock, theBlock->nextBlock);
		//printf("after %s replacement: ", label);
		//printAllMemContents(*mem);
		return theBlock;
		
	}
	else
	{
		if (theBlock->prevBlock == NULL)
		{
			// say we have block t. a->t->z becomes a->newblock->t->z
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, NULL, theBlock);
			*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock, theBlock->nextBlock);
			mem->firstBlock = theBlock->prevBlock;
			return theBlock->prevBlock;
		}
		else
		{
			//// say we have block t. a->t->z becomes a->newblock->t->z

			////malloc space for the process and set it as the next block of the previous block
			//theBlock->prevBlock->nextBlock = malloc(sizeof(block));

			////create the process
			//*(theBlock->prevBlock) = createProcess(processSize, label, theBlock->prevBlock, theBlock);

			////re-create the current proces 
			//*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock->nextBlock, theBlock->nextBlock);

			// say we have block t. a->t->z becomes a->newblock->t->z
			block* prevBlockPointerCopy = theBlock->prevBlock;
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, prevBlockPointerCopy, theBlock);
			*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock, theBlock->nextBlock);

			//set the nextBlock of the block 2 before this one to this one
			prevBlockPointerCopy->nextBlock = (theBlock->prevBlock);
			return theBlock->prevBlock;

		}
		

	}
}

block* findBlock(memory* mem, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->isProcess)
			if (strcmp(b->label, label) == 0)
			{
				return b;
			}
	}

	return NULL;
}

void printAllMemContents(memory mem)
{
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		printBlockContents(*b);
		printf("\n");
	}
}

void printProcessMemContents(memory mem)
{
	bool areProcesses = false;
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->isProcess)
		{
			if (areProcesses)
				printf(" ");
			areProcesses = true;
			printBlockContents(*b);

		}
	}

	if (!areProcesses)
		printf("NONE");
	
	printf("\n");
}

void printEmptyBlockMemContents(memory mem)
{
	bool areEmptyBlocks = false;
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		if (!(b->isProcess))
		{
			if (areEmptyBlocks)
				printf(" ");
			areEmptyBlocks = true;
			printBlockContents(*b);
		}
	}

	if (!areEmptyBlocks)
		printf("FULL");

	printf("\n");
}

#endif //MEMORY_C