#include "block.c"
#include "memory.c"

void splitBlock(block* blockToSplit)
{
	//printf("Splitting block: \n");
	//printBlockContents(*blockToSplit);
	block* sb2 = malloc(sizeof(block));
	block* blockAfterSplit = blockToSplit->nextBlock;
	*blockToSplit = createEmptyBlock(blockToSplit->size / 2, blockToSplit->prevBlock, sb2);
	if (blockAfterSplit != NULL)
		blockAfterSplit->prevBlock = sb2;
	*sb2 = createEmptyBlock(blockToSplit->size, blockToSplit, blockAfterSplit);
	//printf("Results: \n");
	//printBlockContents(*sb2);
	//printBlockContents(*blockToSplit);
	//printf("\n");
}

void splitBlockUntilPieceSize(block* blockToSplit, unsigned long size)
{
	while ((blockToSplit->size) / 2 >= size)
	{
		splitBlock(blockToSplit);
		//printBlockContents(*blockToSplit);
	}
}

block* buddySpawnProcess(memory* mem, block* theBlock, char* label, unsigned long processSize)
{
	if (processSize > theBlock->size)
	{
		printf("Process %s size too big", label);
		return NULL;
	}
	else if (processSize == theBlock->size)
	{

		//printf("before %s replacement: ", label);
		//printAllMemContents(*mem);
		*(theBlock) = createProcess(processSize, label, theBlock->prevBlock, theBlock->nextBlock);
		//printf("after %s replacement: ", label);
		//printAllMemContents(*mem);
		return theBlock;

	}
	else
	{
		if (theBlock->prevBlock == NULL)
		{
			// say we have block t. a->t->z becomes a->newblock->t->z
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, NULL, theBlock);
			*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock, theBlock->nextBlock);
			mem->firstBlock = theBlock->prevBlock;
			theBlock->dummyBlock = true;
			return theBlock->prevBlock;
		}
		else
		{

			// say we have block t. a->t->z becomes a->newblock->t->z
			block* prevBlockPointerCopy = theBlock->prevBlock;
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, prevBlockPointerCopy, theBlock);
			*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock, theBlock->nextBlock);

			//set the nextBlock of the block 2 before this one to this one
			prevBlockPointerCopy->nextBlock = (theBlock->prevBlock);
			theBlock->dummyBlock = true;
			return theBlock->prevBlock;

		}


	}
}

void buddyCleanMemory(memory mem)
{
	bool runWithoutChange = false;
	while (!runWithoutChange)
	{
		runWithoutChange = true;
		for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
		{

			//If we're looking at a free block of memory, and not a process block, and the next block isn't null. We also don't want to look directly at dummy blocks.
			if (!(b->isProcess) && (b->nextBlock != NULL) && !(b->dummyBlock))
			{
				//printf("Blocks being considered: \n");
				//printBlockContents(*b);
				//printBlockContents(*(b->nextBlock));
				//printf("\n");

				//If the next block is a dummy block
				if (b->nextBlock->dummyBlock)
				{
					runWithoutChange = false;
					mergeBlocks(b, b->nextBlock);
				}

				if (!(b->nextBlock->isProcess))
				{
					//if the block is a left block
					if (b->location / b->size % 2 == 0)
					{
						//If the blocks are buddies
						if (b->size == b->nextBlock->size)
						{
							runWithoutChange = false;
							mergeBlocks(b, b->nextBlock);
						}
					}
				}
			}
		}
	}
}



void printBuddyBlockContents(block b)
{
	if (b.isProcess)
		printf("(%s, %lu, %lu)", b.label, b.size, b.location);
	else
	{
		bool isAfterOtherEmptyBlock = false;
		for (block* b2 = (b.prevBlock==NULL? NULL: b.prevBlock); b2 != NULL; b2 = b2->prevBlock)
		{
			if (b2->dummyBlock || !(b2->isProcess))
			{
				isAfterOtherEmptyBlock = true;
				break;
			}

			if (b2->isProcess)
				break;
		}
		if (!isAfterOtherEmptyBlock)
		{
			unsigned long size = 0;
			unsigned long location = b.location;
			for (block* b2 = &b; b2 != NULL; b2 = b2->nextBlock)
			{
				if (b2->isProcess)
					break;
				else
					size += b2->size;
			}
			printf("(%lu, %lu)", size, location);
		}
	}
}

void printBuddyEmptyBlockMemContents(memory mem)
{
	bool areEmptyBlocks = false;
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		if (!(b->isProcess))
		{
			if (areEmptyBlocks)
				printf(" ");
			areEmptyBlocks = true;
			printBuddyBlockContents(*b);
		}
	}

	if (!areEmptyBlocks)
		printf("FULL");

	printf("\n");
}
