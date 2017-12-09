#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifndef FILEIO_C
#define FILEIO_C

/*
*A class for defining methods that pertain to FILE manipulation
*/


/*
* Transfers the contents of inputsStream to outputStream if both are non-null FILE pointers
*/
void transferAllFileContents(FILE* inputStream, FILE* outputStream)
{
	if (inputStream != NULL && outputStream != NULL)
	{
		for (int c = fgetc(inputStream); c != EOF; c = fgetc(inputStream))
		{
			fputc(c, outputStream);
		}
	}
}

/*
*
*/
bool openFile(const char* fileDirectory, const char* fileName, FILE** filePointer, const char* streamAccessType)
{
	char* readmeFS = malloc(strlen(fileDirectory) + strlen(fileName) + 2);
	strcpy(readmeFS, fileDirectory);
	strcat(readmeFS, "/");
	strcat(readmeFS, fileName);
	printf("%s\n", readmeFS);
	*filePointer = fopen(readmeFS, streamAccessType);
	free(readmeFS);
}

#endif //FILEIO_C