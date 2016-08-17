#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "List.h"
#include "../TestingHelper/TestingHelper.h"

/* --- string list helpers --- */

void * myStrdup(void * string) {
	return (void *)strdup((char *)string);
}

int strIsEqual(void * testStr, void * targetStr) {
	char * testS = (char *)testStr;
	char * targetS = (char *)targetStr;

	if (testS == NULL && targetS == NULL)
		return 1;
	else if (!testS || !targetS)
		return 0;

	return (strcmp(testS, targetS) == 0) ? 1 : 0;
}

void printStr(void * str) {
	if (!str)
		return;
	printf("%s, ", (char *)str);
}

void squasher(void * string) {
	if (!string)
		return;
	char * s = (char *)string;
	if (strlen(s) > 0)
		s[0] = '_';
}

/* --- integer list helpers --- */

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
	printf("%d, ", *(int *)integer);
}

void incInt(void * integer) {
	if (!integer)
		return;
	(*(int *)integer)++;
}

/* 
 * ##							 ##
 * ### test suite ###
 * ##							 ##
 */

int main() {
	printf("\nRunning List Test...\n");

	/*
	 * size, append, contains, remove tests
	 */

	List * list = NewList(&myStrdup);

	shouldBe_Int(ListSize(list), 0);
	int size = 0;

	shouldBe_Int(AppendToList(list, "a"), ++size);
	shouldBe_Int(AppendToList(list, "b"), ++size);
	shouldBe_Int(AppendToList(list, "c"), ++size);
	shouldBe_Int(AppendToList(list, "d"), ++size);
	shouldBe_Int(AppendToList(list, "d"), ++size);
	shouldBe_Int(AppendToList(list, "e"), ++size);

	shouldBe_Int(AppendToList(list, NULL), -1);
	shouldBe_Int(AppendToList(NULL, "a"), -1);

	PrintList(list, printStr);

	shouldBe_Str(GetFromList(list, &strIsEqual, "c"), "c");
	shouldBe_Str(GetFromList(list, &strIsEqual, "z"), NULL);

	RemoveFromList(list, &strIsEqual, "a");
	shouldBe_Int(ListSize(list), --size);
	PrintList(list, printStr);

	RemoveFromList(list, &strIsEqual, "e");
	shouldBe_Int(ListSize(list), --size);
	PrintList(list, printStr);

	RemoveFromList(list, &strIsEqual, "d");
	size = size - 2;
	shouldBe_Int(ListSize(list), size);
	PrintList(list, printStr);

	shouldBe_Str((char *)GetFromList(list, &strIsEqual, "d"), NULL);
	shouldBe_Str((char *)GetFromList(list, &strIsEqual, "e"), NULL);
	shouldBe_Str((char *)GetFromList(list, &strIsEqual, "a"), NULL);

	shouldBe_Int(ListContains(list, &strIsEqual, "c"), 1);
	shouldBe_Int(ListContains(list, &strIsEqual, "a"), 0);

	ClearList(list);
	shouldBe_Int(ListSize(list), 0);

	/*
	 * testing head and tail functions
	 */

	int arr[] = {1,3,5,7,9,11};
	int HTsize = sizeof(arr) / sizeof(int);

	List * HTlist = NewList(&myIntDup);
	for (int i = 0; i < HTsize; i++) {
		shouldBe_Int(AppendToList(HTlist, &arr[i]), i + 1);
	}
	PrintList(HTlist, &printInt);

	shouldBe_Int(*(int *)PeekHead(HTlist), 1);
	shouldBe_Int(*(int *)PeekTail(HTlist), 11);

	int * head = (void*)TakeHead(HTlist);
	shouldBe_Int(*head, 1);
	shouldBe_Int(*(int *)PeekHead(HTlist), 3);
	shouldBe_Int(ListSize(HTlist), --HTsize);
	free(head);

	int * tail = (void*)TakeTail(HTlist);
	shouldBe_Int(*tail, 11);
	shouldBe_Int(*(int *)PeekTail(HTlist), 9);
	shouldBe_Int(ListSize(HTlist), --HTsize);
	free(tail);

	PrintList(HTlist, &printInt);

	ListApply(HTlist, &incInt);
	PrintList(HTlist, &printInt);

	DestroyList(HTlist);

	/*
	 * testing apply
	 */

	AppendToList(list, "cat");
	AppendToList(list, "dog");
	AppendToList(list, "hamster");
	AppendToList(list, "cheetah");
	PrintList(list, &printStr);
	ListApply(list, &squasher);
	PrintList(list, &printStr);

	/*
	 * testing iterator
	 */

	ListIterator * iterator = MakeListIterator(list);
	shouldBe_Str((char *)GetCurrentFromIterator(iterator), "_at");
	shouldBe_Str((char *)AdvanceAndGetFromIterator(iterator), "_og");
	shouldBe_Str((char *)AdvanceAndGetFromIterator(iterator), "_amster");
	shouldBe_Str((char *)AdvanceAndGetFromIterator(iterator), "_heetah");
	shouldBe_Str((char *)AdvanceAndGetFromIterator(iterator), NULL);
	DestroyListIterator(iterator);

	/*
	 * clean up!
	 */

	DestroyList(list);

	printf("\nList Tests Pass!\n");
	return 0;
}
