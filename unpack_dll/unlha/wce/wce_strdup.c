//
// wce_strdup.cpp   strdup ä÷êî
//////////////////////////////////////////////////////

//#include <stdlib.h>
//#include <string.h>
#include "wcedef.h"

char* strdup(const char* c)
{
	char* ret = (char*)LocalAlloc(LPTR, strlen(c)+1);
	strcpy(ret, c);
	return ret;
}

