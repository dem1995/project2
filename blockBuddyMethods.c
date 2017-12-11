#include "block.c"

void splitBlock(block* blockToSplit)
{
	printf("Splitting block: \n");
	printBlockContents(*blockToSplit);
	block* sb2 = malloc(sizeof(block));
	block* blockAfterSplit = blockToSplit->nextBlock;
	*blockToSplit = createEmptyBlock(blockToSplit->size / 2, blockToSplit->prevBlock, sb2);
	*sb2 = createEmptyBlock(blockToSplit->size, blockToSplit, blockAfterSplit);
	printf("Results: \n");
	printBlockContents(*sb2);
	printBlockContents(*blockToSplit);
	printf("\n");
}

void splitBlockUntilPieceSize(block* blockToSplit, unsigned long size)
{
	while ((blockToSplit->size) / 2 >= size)
	{
		splitBlock(blockToSplit);
		//printBlockContents(*blockToSplit);
	}
}