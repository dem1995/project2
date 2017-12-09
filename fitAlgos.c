#include "memory.c"


bool firstFitProcess(memory* mem, int size, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size && !(b->isProcess))
		{
			spawnProcess(mem, b, label, size);
			cleanMemory(*mem);
			return true;
		}
	}
	return false;
}

bool bestFitProcess(memory* mem, int size, char* label)
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
		return false;
	else
	{
		spawnProcess(mem, bestFitBlock, label, size);
		cleanMemory(*mem);
		return true;
	}
}

bool nextFitProcess(memory* mem, int size, char* label, int* nextFitCounter)
{
	int origCounter = *nextFitCounter;

	int locCounter=0;
	//Check for indices greater than nextFitCounter
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (locCounter > *nextFitCounter)
		{
			*nextFitCounter = locCounter;

			if (b->size >= size)
			{
				spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				return true;
			}
		}

		locCounter += b->size;
	}

	locCounter = 0;
	for (block* b = mem->firstBlock; b != NULL && *nextFitCounter <= origCounter; locCounter+=b->size, b = b->nextBlock)
	{
		if (locCounter > *nextFitCounter)
		{
			*nextFitCounter = locCounter;

			if (b->size >= size)
			{
				spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				return true;
			}
		}

		locCounter += b->size;
	}

	return false;
	//Check for indices greater than the original value of nextFitCounter	
}


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
