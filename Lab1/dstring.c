#include "dstring.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define _CRT_SECURE_NO_WARNINGS


DString dstring_initialize(const char* str)
{
	char* tmpStr; //creating tmparray

	assert(str != NULL);

	tmpStr = (char*)malloc(strlen(str)+1); //Assigning mem to tmp array
	assert(tmpStr != NULL);

	strcpy_s(tmpStr,strlen(str)+1, str); //copying str to tmp array

	return tmpStr; // returning tmparray
}

int dstring_concatenate(DString* destination, DString source)
{;

	char* tmp;

	int sizeDS = 0;

	assert(destination != NULL);
	assert(*destination != NULL);
	assert(source != NULL);

	sizeDS = strlen(*destination) + strlen(source); //Räknar ut storlek för realloc
	sizeDS++;// sizeDS + 1 för nollterminering

	tmp = (char*)realloc(*destination, sizeDS * sizeof(DString)); //expanding memory
	assert(tmp != NULL);
	
	strcat(tmp, source);

	*destination = tmp;

	return 1; 
}

void dstring_truncate(DString* destination, unsigned int truncatedLength)
{
	char* tmpArr;
	char* tmp;

	assert(destination != NULL);
	assert(*destination != NULL);
	assert(truncatedLength > 0);
	assert(truncatedLength < strlen(*destination));

	tmpArr = (char*)malloc(strlen(*destination) * sizeof(char) + 1); // +1 för \0

	strcpy(tmpArr, *destination); //copy to temporary array
	tmpArr[truncatedLength] = '\0'; //truncating array by placing \0 at new end

	tmp = (char*)realloc(*destination, truncatedLength * sizeof(DString) + 100); //Decreasing memory
	assert(tmp != NULL);

	strcpy(tmp, tmpArr);//copying over from tmp to destinaion

	*destination = tmp;

	free(tmpArr);//freeing memory for tmparr since its of no use
	tmpArr = NULL;
}

void dstring_print(DString str, FILE* textfile)
{
	assert(textfile != NULL);
	assert(str != NULL);

	fprintf(textfile, "%s", str);//printing str
}

void dstring_delete(DString* stringToDelete)
{
	assert(stringToDelete != NULL);

	free(*stringToDelete);//Freeing memory from array
	*stringToDelete = NULL; 
}
