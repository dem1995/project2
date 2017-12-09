#include "fileIOMethods.c"
#include "fitAlgos.c"
#include <stdlib.h>
#include <unistd.h>


#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token sparators

#define KCYN  "\x1B[36m"	//Cyan text
#define RESET "\x1B[0m"	//Reset text color


int nextFitCounter;

int main(int argc, char ** argv)
{
	char buf[MAX_BUFFER];		// line buffer
	char* args[MAX_ARGS];		// pointers to arg strings
	char** arg;					// working pointer thru args
	char* prompt = "==>";		// prompt
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	if (argc < 4)
	{
		printf("Not enough arguments\n");
		return -1;
	}

	//Fit Algorithm Type, space to allocate in memory, filename to draw commands from
	char* fitAlgoChoice = NULL;
	int spaceToAllocate = 0;
	char* fileName = NULL;
	FILE* shellInFP = NULL;

	//If we have enough arguments, get things going; otherwise, end the program
	fitAlgoChoice = argv[1];
	spaceToAllocate = atoi(argv[2]);
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
	if (shellInFP == NULL)
	{
		shellInFP = stdin;
		fprintf(stdout, "There was a problem opening the batch file. Reverting to using standard input.");
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

				if (strcmp(args[0], "REQUEST") == 0)
				{
					int size = atoi(args[2]);
					if (strcmp(fitAlgoChoice, "FIRSTFIT") == 0)
						firstFitProcess(&mem, size, label);
					else if (strcmp(fitAlgoChoice, "BESTFIT") == 0)
						bestFitProcess(&mem, size, label);
					else if (strcmp(fitAlgoChoice, "NEXTFIT") == 0)
						nextFitProcess(&mem, size, label, &nextFitCounter);
					else if (strcmp(fitAlgoChoice, "BUDDY") == 0)
						printf("Buddy");
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

	if (shellInFP != NULL)
		fclose(shellInFP);
}

