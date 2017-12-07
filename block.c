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
		false,
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

void printBlockContents(block b)
{
	printf("Block of size %i. ", b.size);
	if (b.isProcess)
		printf("Block is process of name %s", b.label);
	else
		printf("Block is not a process.");
	printf("\n");
}
