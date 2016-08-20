#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "IntHelper.h"

void * myIntDup(void * integer) {
	if (!integer)
		return NULL;
	int * copy = (int *)calloc(1,sizeof(int));
	assert(copy);
	memcpy(copy, integer, sizeof(int));
	return copy;
}

int myIntCmp(void * testInteger, void * targetInteger) {
	if (testInteger == NULL && targetInteger == NULL)
		return 1;
	else if (!testInteger || !targetInteger)
		return 0;

	return *(int *)testInteger == *(int *)targetInteger;
}

void printInt(void * integer) {
	if (!integer)
		return;
	printf("%d", *(int *)integer);
}

void incInt(void * integer) {
	if (!integer)
		return;
	(*(int *)integer)++;
}
