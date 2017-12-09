#include "file_io.c"
#include "fit_algos.c"
#include <stdlib.h>
#include <unistd.h>

#ifndef ALLOC_MAIN_C
#define ALLOC_MAIN_C

#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token sparators

#define KCYN  "\x1B[36m"	//Cyan text
#define RESET "\x1B[0m"	//Reset text color

unsigned long nextFitCounter = 0;

int main(int argc, char ** argv)
{
	char buf[MAX_BUFFER];		// line buffer
	char* args[MAX_ARGS];		// pointers to arg strings
	char** arg;					// working pointer thru args
	char* prompt = "==>";		// prompt
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	bool showCommands = false;

	if (argc < 4)
	{
		printf("Not enough arguments\n");
		return -1;
	}
	if (argc >= 5)
	{
		if (strcmp(argv[4], "-showYourMoves") == 0)
		{
			showCommands = true;
		}
	}

	//Fit Algorithm Type, space to allocate in memory, filename to draw commands from
	char* fitAlgoChoice = NULL;
	unsigned long spaceToAllocate = 0;
	char* fileName = NULL;
	FILE* shellInFP = NULL;

	//If we have enough arguments, get things going; otherwise, end the program
	fitAlgoChoice = argv[1];
	spaceToAllocate = strtoul(argv[2], NULL, 10);
	fileName = argv[3];


	memory mem = createMemory(spaceToAllocate);


	//Open a file pointer to the file to read from
	if (fileName[0] == '/')
		shellInFP = fopen(fileName, "r");
	else
	{
		printf("%s\n", cwd);
		openFile(cwd, fileName, &shellInFP, "r");
	}


	/* Now for input readin.*/
	while (!feof(shellInFP)) 
	{
		/*Interpret input (executing commands, etc.)*/

		if (fgets(buf, MAX_BUFFER, shellInFP)) //read a line
		{
			/*TOKENIZING THE INPUT*/
			arg = args;
			*arg++ = strtok(buf, SEPARATORS);
			while ((*arg++ = strtok(NULL, SEPARATORS))); // last entry will be NULL	

			//if the input actually has things
			if (args[0])
			{
				if (showCommands)
					printf("%s %s %s\n", args[0], args[1], args[2]);

				char* label = malloc(strlen(args[1]));
				strcpy(label, args[1]);

				/*REQUEST*/
				if (strcmp(args[0], "REQUEST") == 0)
				{

					unsigned long size = atol(arg[2]);
					block* spawnedProcess = NULL;

					if (strcmp(fitAlgoChoice, "FIRSTFIT") == 0)
						spawnedProcess = firstFitProcess(&mem, size, label);
					else if (strcmp(fitAlgoChoice, "BESTFIT") == 0)
						spawnedProcess = bestFitProcess(&mem, size, label);
					else if (strcmp(fitAlgoChoice, "NEXTFIT") == 0)
						spawnedProcess = nextFitProcess(&mem, size, label, &nextFitCounter);
					else if (strcmp(fitAlgoChoice, "BUDDY") == 0)
						printf("Buddy");
					if (spawnedProcess!=NULL)
						printf("ALLOCATED %s %lu\n", label, spawnedProcess->location);
					else
						printf("FAIL REQUEST %s %lu\n", label, size);

				}
				/*RELEASE*/
				else if (strcmp(args[0], "RELEASE") == 0)
				{
					block* b = findBlock(&mem, label);
					if (b == NULL)
						printf("FAIL RELEASE %s\n", label);
					else
					{
						printf("FREE %s %lu %lu\n", label, b->size, b->location);
						releaseBlock(b);
						cleanMemory(mem);
					}
				}
				/*LIST*/
				else if (strcmp(args[0], "LIST") == 0)
				{
					if (strcmp(args[1], "AVAILABLE") == 0)
						printEmptyBlockMemContents(mem);
					else if (strcmp(args[1], "ASSIGNED") == 0)
						printProcessMemContents(mem);
				}
				/*FIND*/
				else if (strcmp(args[0], "FIND") == 0)
				{
					block* b = findBlock(&mem, label);
					if (b == NULL)
						printf("Process %s not found\n", label);
					else
						printf("(%s, %lu, %lu)\n", label, b->size, b->location);
				}
			}
		}
	}

	if (shellInFP != NULL)
		fclose(shellInFP);
}

#endif //ALLOC_MAIN_C