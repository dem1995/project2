#include <stdbool.h>
#include <stdio.h>

typedef struct block
{
	bool isProcess;
	char label[16];
	int size;
	block* prevBlock;
	block* nextBlock;
}block;


block* createEmptyBlock(int size, block* prevBlock, block* nextBlock)
{
	//This is some janky initialization syntax
	block newBlock =
	{
		.isProcess = false,
		.label = 0,
		.size = size,
		.prevBlock = prevBlock,
		.nextBlock = nextBlock
	};

	return &newBlock;
}

block* createProcess(int size, char* label, block* prevBlock, block* nextBlock)
{
	//Might get some string pointer errors here
	block newProcess =
	{
		.isProcess = false,
		.label = label,
		.size = size,
		.prevBlock = prevBlock,
		.nextBlock = nextBlock
	};

	return &newProcess;
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
