#include "memory.c"
#include "fileIOMethods.c"
#include <stdlib.h>
#include <unistd.h>



#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token sparators

#define KCYN  "\x1B[36m"	//Cyan text
#define RESET "\x1B[0m"	//Reset text color

bool firstFitProcess(memory* mem, int size, char* label);
bool bestFitProcess(memory* mem, int size, char* label);
bool nextFitProcess(memory* mem, int size, char* label, int* nextFitCounter);

int nextFitCounter;
FILE* shellInFP = NULL;



int main(int argc, char ** argv) 
{
	char buf[MAX_BUFFER];		// line buffer
	char* args[MAX_ARGS];		// pointers to arg strings
	char** arg;					// working pointer thru args
	char* prompt = "==>";		// prompt
	char cwd[1024];



	int spaceToAllocate = 0;
	if (argc > 1)
		spaceToAllocate = atoi(argv[2]);
	if (argc > 2)
	{	
		getcwd(cwd, sizeof(cwd));		
	}

	memory mem = createMemory(spaceToAllocate);


	if (argv[2] != NULL)
	{
		if (argv[1][0] == '/')
			shellInFP = fopen(argv[3], "r");
		else
		{
			printf("%s\n", cwd);
			openFile(cwd, argv[3], &shellInFP, "r");
		}
		if (shellInFP == NULL)
		{
			shellInFP = stdin;
			fprintf(stdout, "There was a problem opening the batch file. Reverting to using standard input.");
		}
	}

	/* Now for input readin. Keep reading input until "quit" command or eof of redirected input */
	while (!feof(shellInFP)) {

		//Prints the current directory to stdout
		if (shellInFP == stdin)
			fprintf(stdout, KCYN"%s"RESET"%s ", getenv("PWD"), prompt); //write prompt

																			/*Interpret input (executing commands, etc.)*/
		if (fgets(buf, MAX_BUFFER, shellInFP)) //read a line
		{

			/*TOKENIZING THE INPUT*/
			arg = args;
			*arg++ = strtok(buf, SEPARATORS);
			while ((*arg++ = strtok(NULL, SEPARATORS))); // last entry will be NULL	

														 //if the user's input actually has things
			if (args[0])
			{
				printf("%s_%s_%s\n", args[0], args[1], args[2]);

				char* label = malloc(strlen(args[1]));
				strcpy(label, args[1]);

				if (strcmp(args[0], "REQUEST")==0)
				{

					firstFitProcess(&mem, atoi(args[2]), label);
					//printAllMemContents(mem);
				}
				else if (strcmp(args[0], "RELEASE") == 0)
				{
					releaseProcess(&mem, label);
				}
				else if (strcmp(args[0], "LIST") == 0)
				{
					if (strcmp(args[1], "AVAILABLE") == 0)
						printEmptyBlockMemContents(mem);
					else if (strcmp(args[1], "ASSIGNED") == 0)
						printProcessMemContents(mem);
				}
				else if (strcmp(args[0], "FIND") == 0)
				{
					;
				}
			}
		}
	}

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
	cleanMemory(*mem);
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


bool buddyFit(memory* mem, int size, char* label)
{
	splitBlock(mem->firstBlock);
}
