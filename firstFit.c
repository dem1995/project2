#include "memory.c"

void main()
{
	memory mem = createMemory(20);

	printf("hi");

	printMemContents(mem);
	return 0;
}


void firstFitProcess(memory* mem, block* blo)
{
	for (block* b = mem->firstBlock; b != NULL; b = b->nextBlock)
	{
		if (b->size >= blo->size)
		{
			//spawnProcess(mem, blo, )
		}
	}
}
