#include "memory.c"
#include <stdlib.h>

void main()
{
	memory mem = createMemory(20);

	
	spawnProcess(&mem, mem.firstBlock, "", 15);

	printMemContents(mem);
	freeMemory(mem);
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
