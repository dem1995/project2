#include "memory.c"
#include "block_buddy_methods.c"


//Algorithms for fitting processes into memory.
#ifndef FIT_ALGOS_C
#define FIT_ALGOS_C

block* firstFitProcess(memory* mem, unsigned long size, char* label)
{
	//cycles through blocks
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		//the first non-process block we hit that is large enough will house the spawned process
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
		//If the encountered size is large enough
		if (b->size >= size)
			//And the block is not a process
			if(!(b->isProcess))
				//and either this is smaller than the previously-found block size, or the previously found minimum block size is 0
				if (minAccomodatingBlockSize == 0 || b->size < minAccomodatingBlockSize)
				{
					minAccomodatingBlockSize = b->size;
					bestFitBlock = b;
				}		
	}

	//If there were no sufficiently-large blocks
	if (minAccomodatingBlockSize == 0)
		return NULL;
	//Spawn the process in the smallest possible block (possible being "capable of being spawned in given this process size)
	else
	{
		block* spawnedProcess = spawnProcess(mem, bestFitBlock, label, size);
		cleanMemory(*mem);
		return spawnedProcess;
	}
}

block* nextFitProcess(memory* mem, unsigned long size, char* label, unsigned long* nextFitCounter)
{
	//Check part of memory to the right of the original cursor location given by nextFitCounter.
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		//if the location is to the right of the location given by nextFitCounter
		if (b->location > *nextFitCounter)
		{
			//if the block at that location is large enough and is not a process
			if (b->size >= size && !(b->isProcess))
			{
				//spawn the process in that location
				block* spawnedProcess = spawnProcess(mem, b, label, size);
				cleanMemory(*mem);
				*nextFitCounter = spawnedProcess->location;
				return spawnedProcess;
			}
		}
	}

	//Check part of memory to the left of the original cursor location given by nextFitCounter.
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		//if the location is to the left of the location given by nextFitCounter
		if (b->location <= *nextFitCounter)
		{
			//If the block at that location is large enough and is not a process
			if (b->size >= size && !(b->isProcess))
			{
				//spawn the process at that location.
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

	//Cycle through the non-dummy blocks ("dummy" blocks being blocks that a process has reserved, but is not occupying), finding the minimum accomodating block size
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		//If the block is large enough, is not a process, and is not a dummy block
		if (b->size >= size && !(b->isProcess) && ! (b->dummyBlock))
		{
			//and either this is smaller than the previously-found block size, or the previously found minimum block size is 0
			if (b->size < curSmallestAccoBlock || curSmallestAccoBlock == 0)
			{
				curSmallestAccoBlock = b->size;
				blockToBreak = b;
			}
		}
	}

	//if we found a block
	if (blockToBreak!=NULL)
	{
		//Split the block, dividing it into two equal pieces repeatedly (and then considering the block to be the leftern piece), until we can't split it anymore without being unable to house the process in it.
		splitBlockUntilPieceSize(blockToBreak, size, *mem);
		//Spawn the process in the block.
		spawnedProcess = buddySpawnProcess(mem, blockToBreak, label, size);
	}

	//Clean the memory, avoiding actually combining blocks that aren't buddies or part of a single buddy.
	buddyCleanMemory(*mem);
	return spawnedProcess;
}

#endif //FIT_ALGOS_C