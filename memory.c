#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.c"

//General structs and methods for representing and manipulating memory. 
#ifndef MEMORY_C
#define MEMORY_C

typedef struct memory
{
	unsigned long totalMemory;	//The total number of bytes this memory structure represents
	block* firstBlock;			//The first Block in memory (left-most, if it exists it always has relative position 0).
}memory;

void printAllMemContents(memory mem);

//Creates memory with "size bytes". Blocks are kept track of as part of a linked list.
memory createMemory(unsigned long size)
{
	memory newMemory = {
		//.totalMemory = size,
		size,
		//.firstBlock = malloc(sizeof(block))
		malloc(sizeof(block))
	};

	//spawn an empty block in memory representing all it can hold.
	*(newMemory.firstBlock) = createEmptyBlock(size, NULL, NULL);
	return newMemory;
}

//Cleans the memory by merging adjacent empty blocks.
void cleanMemory(memory mem)
{
	//keeps track of whether blocks got merged in the last run-through
	bool runWithoutChange = false;
	//If it is not true that no blocks were merged in the last run-through
	while (!runWithoutChange)
	{
		//Assume it will be true this time. This will be changed to false if a change actually occurs.
		runWithoutChange = true;
		//Cycle through the blocks
		for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
		{
			//If we're looking at a free block of memory, and not a process block, and the next block isn't null
			if (!(b->isProcess) && (b->nextBlock != NULL))
			{
				//If the block after this one is not a process
				if (!(b->nextBlock->isProcess))
				{
					runWithoutChange = false;
					//Combine the blocks
					mergeBlocks(b, b->nextBlock);
				}
			}
		}
	}
}


//Free all the memory (malloced blocks and malloced block labels)
void freeMemory(memory mem)
{
	//cycle through the blocks
	for (block* b = mem.firstBlock; b != NULL;)
	{

		block* nextBlockPtrCopy = b->nextBlock;
		
		//if the block exists
		if (b != NULL)
		{
			//if its label exists
			if (b->label != NULL)
				free(b->label);
			free(b);
		}

		b = nextBlockPtrCopy;
	}
}

//Spawns a process in a block of memory, breaking the block in twain (into a process block and a non-process block)
block* spawnProcess(memory* mem, block* theBlock, char* label, unsigned long processSize)
{
	//if the process size is small enough
	if (processSize > theBlock->size)
	{
		return NULL;
	}
	else if (processSize == theBlock->size)
	{
		*(theBlock) = createProcess(processSize, label, theBlock->prevBlock, theBlock->nextBlock);
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

//Finds a block in memory with the given label and returns a pointer to it. Retruns NULL if no such block can be found
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

//Prints all the blocks in memory
void printAllMemContents(memory mem)
{
	bool spaceBool = false;
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		if (spaceBool)
			printf(" ");
		spaceBool = true;
		printBlockContents(*b);
		printf("(Is Process: %s, Label: %s, Size: %lu, Location: %lu, Dummy: %s)\n", b->isProcess ? "true" : "false", b->label, b->size, b->location, b->dummyBlock ? "true" : "false");
		bool pbin = b->prevBlock == NULL;
		bool nbin = b->nextBlock == NULL;
		printf("Prev : (%lu, %lu) Next : (%lu, %lu)\n", pbin ? 1 : b->prevBlock->size, pbin ? 1 : b->prevBlock->location, nbin ? 1 : b->nextBlock->size, nbin ? 1 : b->nextBlock->location);
	}
	printf("\n");
}

//Prints all the processes in memory
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

//Prints all the non-process blocks in memory.
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