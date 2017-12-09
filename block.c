#include <stdbool.h>
#include <stdio.h>

#ifndef BLOCK_C
#define BLOCK_C

typedef struct block
{
	bool isProcess;	//whether the block is given to a process or not
	char* label;	//if the block is a process, the process name
	unsigned long size;		//the number of bytes the block has
	unsigned long location;
	struct block* prevBlock;
	struct block* nextBlock;
}block;


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
		nextBlock
	};

	return newBlock;
}

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
		nextBlock
	};

	return newProcess;
}

void releaseBlock(block* blockToRelease)
{
	(*blockToRelease) = createEmptyBlock(blockToRelease->size, blockToRelease->prevBlock, blockToRelease->nextBlock);
}


void printBlockContents(block b)
{

	if (b.isProcess)
		printf("(%s, %lu, %lu)", b.label, b.size, b.location);
	else
		printf("(%lu, %lu)", b.size, b.location);
	
}

#endif //BLOCK_C