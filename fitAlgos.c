#include "memory.c"


block* firstFitProcess(memory* mem, int size, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size && !(b->isProcess))
		{
			spawnProcess(mem, b, label, size);
			cleanMemory(*mem);
			return b;
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
		spawnProcess(mem, bestFitBlock, label, size);
		cleanMemory(*mem);
		return bestFitBlock;
	}
}


//
//bool nextFitProcess(memory* mem, int size, char* label, int* nextFitCounter)
//{
//
//	//Check part of memory to the right of the original cursor location.
//	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
//	{
//		if (b->location > *nextFitCounter)
//		{
//			if (b->size >= size && !(b->isProcess))
//			{
//				spawnProcess(mem, b, label, size);
//				cleanMemory(*mem);
//				*nextFitCounter = b->location;
//				return true;
//			}
//		}
//	}
//
//	//Check part of memory to the left of the original cursor location.
//	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
//	{
//		if (b->location <= *nextFitCounter)
//		{
//			if (b->size >= size && !(b->isProcess))
//			{
//				spawnProcess(mem, b, label, size);
//				cleanMemory(*mem);
//				*nextFitCounter = b->location;
//				return true;
//			}
//		}
//		//If we've passed the original cursor location, then there's no room in memory for this process.
//		else
//			break;
//	}
//
//	return false;
//	//Check for indices greater than the original value of nextFitCounter	
//}
block* nextFitProcess(memory* mem, int size, char* label, int* nextFitCounter)
{

	int currentMemIndex = 0;

	//Check part of memory to the right of the original cursor location.
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (currentMemIndex > *nextFitCounter)
		{
			if (b->size >= size && !(b->isProcess))
			{
				spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				*nextFitCounter = currentMemIndex;
				//printf("curMemIndex: %i, b->location: %i\n", currentMemIndex, b->location);
				return b;
			}
		}
		//Increment (to track) the current index of memory
		currentMemIndex += b->size;
	}

	currentMemIndex = 0;
	//Check part of memory to the left of the original cursor location.
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (currentMemIndex <= *nextFitCounter)
		{
			if (b->size >= size && !(b->isProcess))
			{
				spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				*nextFitCounter = currentMemIndex;
				//printf("curMemIndex: %i, b->location: %i\n", currentMemIndex, b->location);
				return b;
			}
		}
		//If we've passed the original cursor location, then there's no room in memory for this process.
		else
			break;

		currentMemIndex += b->size;
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
