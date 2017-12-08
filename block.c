#include <stdbool.h>
#include <stdio.h>

typedef struct block
{
	bool isProcess;
	char* label;
	int size;
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

int getRelativeLocation(block* b)
{
	int i = 0;
	for (block* c = b->prevBlock; c != NULL; c = c->prevBlock)
	{
		i += (c->size);
	}

	return i;
}

void printBlockContents(block b)
{

	printf("(%s, %i, %i)", b.label, b.size, getRelativeLocation(&b));
	/*
	printf("Block of size %i. ", b.size);
	if (b.isProcess)
		printf("Block is process of name %s", b.label);
	else
		printf("Block is not a process.");
	printf("\n");
	*/

}


