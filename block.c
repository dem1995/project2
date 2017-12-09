#include <stdbool.h>
#include <stdio.h>

typedef struct block
{
	bool isProcess;
	char* label;
	int size;

	int location;
	struct block* prevBlock;
	struct block* nextBlock;
}block;


block createEmptyBlock(int size, block* prevBlock, block* nextBlock)
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

block createProcess(int size, char* label, block* prevBlock, block* nextBlock)
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
		printf("(%s, %i, %i)", b.label, b.size, b.location);
	else
		printf("(%i, %i)", b.size, b.location);
	
}


