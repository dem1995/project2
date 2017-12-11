#include "memory.c"
#include "block_buddy_methods.c"

#ifndef FIT_ALGOS_C
#define FIT_ALGOS_C

block* firstFitProcess(memory* mem, unsigned long size, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size && !(b->isProcess))
		{
			block* spawnedProcess = spawnProcess(mem, b, label, size);
			cleanMemory(*mem);
			return spawnedProcess;
		}
	}
	return NULL;
}

block* bestFitProcess(memory* mem, unsigned long size, char* label)
{
	unsigned long minAccomodatingBlockSize = 0;
	block* bestFitBlock = NULL;

	//Cycle through the blocks, finding the minimum accomodating block size
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size)
			if(!(b->isProcess))
				if (minAccomodatingBlockSize == 0 || b->size < minAccomodatingBlockSize)
				{
					minAccomodatingBlockSize = b->size;
					bestFitBlock = b;
				}		
	}

	//If there were no sufficiently-large blocks
	if (minAccomodatingBlockSize == 0)
		return NULL;
	else
	{
		block* spawnedProcess = spawnProcess(mem, bestFitBlock, label, size);
		cleanMemory(*mem);
		return spawnedProcess;
	}
}

block* nextFitProcess(memory* mem, unsigned long size, char* label, unsigned long* nextFitCounter)
{
	//Check part of memory to the right of the original cursor location.
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->location > *nextFitCounter)
		{
			if (b->size >= size && !(b->isProcess))
			{
				block* spawnedProcess = spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				*nextFitCounter = spawnedProcess->location;
				return spawnedProcess;
			}
		}
	}

	//Check part of memory to the left of the original cursor location.
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->location <= *nextFitCounter)
		{
			if (b->size >= size && !(b->isProcess))
			{
				block* spawnedProcess = spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				*nextFitCounter = spawnedProcess->location;
				return spawnedProcess;
			}
			//If we've passed the original cursor location, then there's no room in memory for this process.
			else
				break;
		}
	}

	return NULL;
	//Check for indices greater than the original value of nextFitCounter	
}


block* buddyFitProcess(memory* mem, unsigned long size, char* label)
{
	unsigned long curSmallestAccoBlock=0;
	block* blockToBreak = NULL;
	block* spawnedProcess = NULL;
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size > size && !(b->isProcess) && ! (b->dummyBlock))
		{
			if (b->size < curSmallestAccoBlock || curSmallestAccoBlock == 0)
			{
				curSmallestAccoBlock = b->size;
				blockToBreak = b;
			}
		}
	}

	if (blockToBreak != NULL)
	{
		//printf("The block we're breaking for process %s is ", label);
		//printBlockContents(*blockToBreak);
	}

	//printAllMemContents(*mem);

	if (blockToBreak!=NULL)
	{
		splitBlockUntilPieceSize(blockToBreak, size);
		//printf("AFter breaking, but before spawning: \n");
		//printAllMemContents(*mem);
		spawnedProcess = buddySpawnProcess(mem, blockToBreak, label, size);
	}

	//printf("AFter spawning, but before cleaning: \n");
	//printAllMemContents(*mem);

	buddyCleanMemory(*mem);

	//printf("After cleaning: \n");
	//printAllMemContents(*mem);

	return spawnedProcess;
}

#endif //FIT_ALGOS_C