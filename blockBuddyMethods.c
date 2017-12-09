#include "block.c"

void splitBlock(block* blockToSplit)
{
	block* sb2 = malloc(sizeof(block));
	*sb2 = createEmptyBlock(blockToSplit->size / 2, blockToSplit, blockToSplit->nextBlock);
	blockToSplit->size /= 2;
	blockToSplit->nextBlock = sb2;
}



void splitBlockUntilPieceSize(block* blockToSplit, unsigned long size)
{
	while ((blockToSplit->size) / 2 >= size)
	{
		splitBlock(blockToSplit);
		printf("Split block:");
		printBlockContents(*blockToSplit);
	}
}