#include "memory.c"

#ifndef FIT_ALGOS_C
#define FIT_ALGOS_C

block* firstFitProcess(memory* mem, int size, char* label)
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

block* bestFitProcess(memory* mem, int size, char* label)
{
	int minAccomodatingBlockSize = -1;
	block* bestFitBlock = NULL;

	//Cycle through the blocks, finding the minimum accomodating block size
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size)
			if(!(b->isProcess))
				if (minAccomodatingBlockSize < 0 || b->size < minAccomodatingBlockSize)
				{
					minAccomodatingBlockSize = b->size;
					bestFitBlock = b;
				}		
	}

	//If there were no sufficiently-large blocks
	if (minAccomodatingBlockSize < 0)
		return NULL;
	else
	{
		block* spawnedProcess = spawnProcess(mem, bestFitBlock, label, size);
		cleanMemory(*mem);
		return spawnedProcess;
	}
}

block* nextFitProcess(memory* mem, int size, char* label, int* nextFitCounter)
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
				*nextFitCounter = b->location;
				//printf("curMemIndex: %i, b->location: %i\n", currentMemIndex, b->location);
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
				*nextFitCounter = b->location;
				//printf("curMemIndex: %i, b->location: %i\n", currentMemIndex, b->location);
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

/*
bool buddyFit(memory* mem, int size, char* label)
{
	int curSmallestAccoBlock=-1;
	block* blockToBreak = NULL;
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size > size && !(b->isProcess))
		{
			if (b->size < curSmallestAccoBlock || curSmallestAccoBlock < 0)
			{
				curSmallestAccoBlock = b->size;
				blockToBreak = b;
			}
		}
	}

	if (blockToBreak!=NULL)
	{
		splitBlockUntilPieceSize(blockToBreak, size);

	}
	//TODO delete this line
	return true;

}
*/
#endif FIT_ALGOS_C