#include "memory.c"
#include <stdlib.h>
#include <unistd.h>

bool firstFitProcess(memory* mem, int size, char* label);
bool bestFitProcess(memory* mem, int size, char* label);
bool nextFitProcess(memory* mem, int size, char* label, int* nextFitCounter);

int nextFitCounter;


int main(int argc, char ** argv) 
{
	int spaceToAllocate = 0;
	if (argc > 1)
		spaceToAllocate = atoi(argv[2]);

	if (argc > 2)
	{
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf(cwd);
	}
}
void main2()
{
	printf("Creating memory of size 20\n");
	memory mem = createMemory(20);
	printf("\n");

	printf("FirstFit adding size 13 fred\n");
	firstFitProcess(&mem, 13, "fred");
	printMemContents(mem);
	printf("\n");

	printf("NextFit adding size 6 geore\n");
	nextFitProcess(&mem, 6, "george", &nextFitCounter);
	printMemContents(mem);
	printf("\n");

	printf("Releasing fred\n");
	releaseProcess(&mem, "fred");
	cleanMemory(mem);
	printMemContents(mem);
	printf("\n");

	printf("NextFit adding harold\n");
	nextFitProcess(&mem, 1, "harold", &nextFitCounter);
	printMemContents(mem);
	printf("\n");

	printf("End");
	freeMemory(mem);
	return 0;
}

void main3()
{
	printf("Creating memory of size 20\n");
	memory mem = createMemory(20);
	printf("\n");

	printf("FirstFit adding size 13 fred\n");
	firstFitProcess(&mem, 13, "fred");	
	printMemContents(mem);
	printf("\n");

	printf("FirstFit adding size 6 geore\n");
	firstFitProcess(&mem, 6, "george");
	printMemContents(mem);
	printf("\n");
	
	printf("Releasing fred\n");
	releaseProcess(&mem, "fred");
	cleanMemory(mem);
	printMemContents(mem);
	printf("\n");

	printf("BestFit adding harold\n");
	bestFitProcess(&mem, 1, "harold");
	printMemContents(mem);
	printf("\n");

	printf("End");
	freeMemory(mem);
	return 0;
}


bool firstFitProcess(memory* mem, int size, char* label)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= size && !(b->isProcess))
		{
			spawnProcess(mem, b, label, size);
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
				return true;
			}
		}

		locCounter += b->size;
	}

	return false;
	//Check for indices greater than the original value of nextFitCounter	
}
