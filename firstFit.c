#include "memory.c"
#include <stdlib.h>

void main()
{
	printf("hi");

	memory mem = createMemory(20);


	printMemContents(mem);
	free(mem.firstBlock);
	printf("hi");
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
