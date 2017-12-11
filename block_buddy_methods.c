#include "block.c"
#include "memory.c"

//Methods for Buddy Block memory allocation
#ifndef BUDDY_BLOCK_METHODS_C
#define BUDDY_BLOCK_METHODS_C

//Splits a block into two.
void splitBlock(block* blockToSplit)
{
	block* sb2 = malloc(sizeof(block));
	block* blockAfterSplit = blockToSplit->nextBlock;
	*blockToSplit = createEmptyBlock(blockToSplit->size / 2, blockToSplit->prevBlock, sb2);
	if (blockAfterSplit != NULL)
		blockAfterSplit->prevBlock = sb2;
	*sb2 = createEmptyBlock(blockToSplit->size, blockToSplit, blockAfterSplit);
}

//Keeps splitting a block into two pieces (splitting the leftmost block at each step) until the leftmost block is no longer able to be split without being too small to house a process of /size/ bytes.
void splitBlockUntilPieceSize(block* blockToSplit, unsigned long size, memory mem)
{
	while ((blockToSplit->size) / 2 >= size)
	{
		splitBlock(blockToSplit);
	}
}

//Spawns a process, marking the leftover memory space as dummy space (memory that's not used by the spawned process, but is nevertheless reserved).
block* buddySpawnProcess(memory* mem, block* theBlock, char* label, unsigned long processSize)
{
	if (processSize > theBlock->size)
	{
		return NULL;
	}
	else if (processSize == theBlock->size)
	{
		*(theBlock) = createProcess(processSize, label, theBlock->prevBlock, theBlock->nextBlock);
		return theBlock;

	}
	else
	{
		if (theBlock->prevBlock == NULL)
		{
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, NULL, theBlock);
			*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock, theBlock->nextBlock);
			mem->firstBlock = theBlock->prevBlock;
			theBlock->dummyBlock = true;
			return theBlock->prevBlock;
		}
		else
		{
			block* prevBlockPointerCopy = theBlock->prevBlock;
			theBlock->prevBlock = malloc(sizeof(block));
			*(theBlock->prevBlock) = createProcess(processSize, label, prevBlockPointerCopy, theBlock);
			*theBlock = createEmptyBlock(theBlock->size - processSize, theBlock->prevBlock, theBlock->nextBlock);

			prevBlockPointerCopy->nextBlock = (theBlock->prevBlock);
			theBlock->dummyBlock = true;
			return theBlock->prevBlock;

		}


	}
}

//Cleans the memory by merging empty buddies.
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

//Prints a buddy block's contents (if it's the first non-process block in a chain of process blocks, prints as if it were a joined version of all those non-process blocks).
bool printBuddyBlockContents(block b)
{
	//If b is a process
	if (b.isProcess)
	{
		printf("(%s, %lu, %lu)", b.label, b.size, b.location);
		return true;
	}
	//If b's not a process, we need to figure out whether it would be "absorbed" by a previous adjacent empty memory blocks if this weren't a buddy-fit system (in which case it should have been summed up and printed as if it were part of an earlier block).
	else
	{
		//keeps track of whether this block occurs after an earlier empty block in a chain without any processes
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
		//If this is the first element of a chain of process-less blocks, print it as if it were a combined block of it and all of the process-less blocks after it (up until you hit another process block).
		if (!isAfterOtherEmptyBlock)
		{
			//The size of the sum of the sizes of the blocks in a chain of blocks starting from this one and extending down the line up until a process block is hit (but not including that process block)
			unsigned long size = 0;
			//The location of this block (and, since this is the first block in the chain of non-process blocks, the location of the hypothetical merged non-process block this would be a part of)
			unsigned long location = b.location;
			for (block* b2 = &b; b2 != NULL; b2 = b2->nextBlock)
			{
				if (b2->isProcess)
					break;
				else
					size += b2->size;
			}
			printf("(%lu, %lu)", size, location);
			return true;
		}
	}
	return false;
}

//Prints all of the blocks in memory as if non-process blocks were merged with any they form an unbroken chain of non-process blocks with.
void printBuddyEmptyBlockMemContents(memory mem)
{
	bool areEmptyBlocks = false;
	bool hasRun = false;
	for (block* b = mem.firstBlock; b != NULL; b = b->nextBlock)
	{
		if (!(b->isProcess))
		{
			if (areEmptyBlocks)
				printf(" ");
			areEmptyBlocks = printBuddyBlockContents(*b);
			if (areEmptyBlocks)
				hasRun = true;
		}
	}

	if (!hasRun)
		printf("FULL");

	printf("\n");
}

#endif //BUDDY_BLOCK_METHODS_C
