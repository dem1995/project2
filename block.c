#include <stdbool.h>
#include <stdio.h>

//typedef doesn't do anything in gcc
struct block
{
	bool isProcess;
	char* label;
	int size;
	block* prevBlock;
	block* nextBlock;
};


block* createEmptyBlock(int size, block* prevBlock, block* nextBlock)
{
	//Initialize as a pointer to begin with
	block *newBlock = new block
	{
		.isProcess = false,
		.label = NULL,
		.size = size,
		.prevBlock = prevBlock,
		.nextBlock = nextBlock
	};

	return newBlock;
}

block* createProcess(int size, char* label, block* prevBlock, block* nextBlock)
{
	//Same as before
	block *newProcess = new block
	{
		.isProcess = false,
		.label = label,
		.size = size,
		.prevBlock = prevBlock,
		.nextBlock = nextBlock
	};

	return newProcess;
}

void printBlockContents(block b)
{
	printf("Block of size %i. ", b.size);
	if (b.isProcess)
		printf("Block is process of name %s", b.label);
	else
		printf("Block is not a process.");
	printf("\n");
}
