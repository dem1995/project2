#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifndef FILEIO_C
#define FILEIO_C

/*
*A class for defining methods that pertain to FILE manipulation
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