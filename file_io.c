#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//Methods for file i/o

#ifndef FILEIO_C
#define FILEIO_C

//Attempts to open a file with the given directory into the File Pointer pointed to by filePointer.
bool openFile(const char* fileDirectory, const char* fileName, FILE** filePointer, const char* streamAccessType)
{
	char* readmeFS = malloc(strlen(fileDirectory) + strlen(fileName) + 2);
	strcpy(readmeFS, fileDirectory);
	strcat(readmeFS, "/");
	strcat(readmeFS, fileName);
	printf("%s\n", readmeFS);
	*filePointer = fopen(readmeFS, streamAccessType);
	free(readmeFS);
	return true;
}

#endif //FILEIO_C