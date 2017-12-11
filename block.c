#include <stdbool.h>
#include <stdio.h>

//General structs and methods for representing and manipulating blocks
#ifndef BLOCK_C
#define BLOCK_C

//Struct for representing a block or process in memory (as well as indicating adjacent blocks/processes)
typedef struct block
{
	bool isProcess;				//Whether the block is a process (non-free)
	char* label;				//If the block is a process, this is the associated program name
	unsigned long size;			//The number of bytes in the block (or locked-down bytes, in the case of the buddy system)
	unsigned long location;		//The relative location of this block within the memory struct instance that contains it.
	struct block* prevBlock;	//A pointer to the previous block in memory. If there is none, this is a null pointer.
	struct block* nextBlock;	//A pointer to the next block in memory. If there is none, this is a null pointer.
	bool dummyBlock;			//Keeps track of whether this empty block is actually reserved by a process in the Buddy system by virtue of internal fragmentation.
}block;


//Creates a non-process block with /size/ bytes
block createEmptyBlock(unsigned long size, block* prevBlock, block* nextBlock)
{
	block newBlock =
	{
		//.isProcess = false,
		false,
		//.label = 0,
		NULL,
		//.size = size,
		size,
		//.location = prevBlock==NULL?0:(prevBlock->location+prevBlock->size);
		prevBlock == NULL ? 0 : (prevBlock->location + prevBlock->size),
		//.prevBlock = prevBlock,
		prevBlock,
		//.nextBlock = nextBlock
		nextBlock,
		//.buddyVal = 0 
		false
	};

	return newBlock;
}

//Creates a process block named /label/ with /size/ bytes
block createProcess(unsigned long size, char* label, block* prevBlock, block* nextBlock)
{
	//Might get some string pointer errors here
	block newProcess = 
	{
		//.isProcess = false,
		true,
		//.label = label,
		label,
		//.size = size,
		size,
		//.location = prevBlock==NULL?0:(prevBlock->location+prevBlock->size);
		prevBlock == NULL ? 0 : (prevBlock->location + prevBlock->size),
		//.prevBlock = prevBlock,
		prevBlock,
		//.nextBlock = nextBlock
		nextBlock,
		//.buddyVal = 0
		false
	};

	return newProcess;
}

block* mergeBlocks(block* thisBlock, block* nextBlock)
{
	thisBlock->size += nextBlock->size;
	thisBlock->nextBlock = nextBlock->nextBlock;
	free(nextBlock);
	if (thisBlock->nextBlock != NULL)
		thisBlock->nextBlock->prevBlock = thisBlock;
	return thisBlock;
}


/*Converts a process block to a non-process block, simulating releasing memory*/
void releaseBlock(block* blockToRelease)
{
	if (blockToRelease->label!=NULL)
		free(blockToRelease->label);
	(*blockToRelease) = createEmptyBlock(blockToRelease->size, blockToRelease->prevBlock, blockToRelease->nextBlock);
}

//Prints the contents of the block in the form (label, size, location) if it is a process; (size, location), otherwise
void printBlockContents(block b)
{

	if (b.isProcess)
		printf("(%s, %lu, %lu)", b.label, b.size, b.location);
	else
		printf("(%lu, %lu)", b.size, b.location);
	
}

#endif //BLOCK_C