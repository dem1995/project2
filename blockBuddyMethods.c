#include "block.c"

void splitBlock(block* blockToSplit)
{
	block* sb2 = malloc(sizeof(block));
	*blockToSplit = createEmptyBlock(blockToSplit->size / 2, blockToSplit->prevBlock, sb2);
	*sb2 = createEmptyBlock(blockToSplit->size / 2, blockToSplit, blockToSplit->nextBlock);
}

void splitBlockUntilPieceSize(block* blockToSplit, unsigned long size)
{
	while ((blockToSplit->size) / 2 >= size)
	{
		splitBlock(blockToSplit);
		printBlockContents(*blockToSplit);
	}
}