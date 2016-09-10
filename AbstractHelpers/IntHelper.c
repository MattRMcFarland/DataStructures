#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "IntHelper.h"

#include "../List/List.h"

void * myIntDup(void * integer) {
	if (!integer)
		return NULL;
	int * copy = (int *)calloc(1,sizeof(int));
	assert(copy);
	memcpy(copy, integer, sizeof(int));
	return copy;
}

int myIntIsEqual(void * testInteger, void * targetInteger) {
	if (testInteger == NULL && targetInteger == NULL)
		return 1;
	else if (!testInteger || !targetInteger)
		return 0;

	return *(int *)testInteger == *(int *)targetInteger;
}

// NULL is considered LEAST value possible
int myIntCompare(const void * a1, const void * a2) {
	int * arg1 = *(int **)a1;
	int * arg2 = *(int **)a2;

	if (arg1 == NULL && arg2 == NULL)
		return 0;
	else if (!arg1 && arg2 != NULL)
		return -1;
	else if (!arg2 && arg1 != NULL)
		return 1;

	return *(int *)arg1 - *(int *)arg2;
}


void printInt(void * integer) {
	if (!integer)
		return;
	printf("%d", *(int *)integer);
}

void printIntList(List * list) {
	PrintList(list, &printInt);
}

void incInt(void * integer) {
	if (!integer)
		return;
	(*(int *)integer)++;
}
