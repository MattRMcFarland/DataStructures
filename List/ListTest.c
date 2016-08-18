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

List * MakeIntListFromArr(int arr[], int size) {
	if (!arr || size < 0)
		return NULL;

	List * new = NewList(&myIntDup);
	for (int i = 0; i < size; i++) {
		AppendToList(new,(void *)&arr[i]);
	}
	return new;
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

	int arr[] = {1, 3, 5, 7, 9, 11};
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

	ClearList(HTlist);
	shouldBe_Int((int)PeekHead(HTlist), (int)NULL);
	shouldBe_Int((int)PeekTail(HTlist), (int)NULL);
	shouldBe_Int((int)TakeHead(HTlist), (int)NULL);
	shouldBe_Int((int)TakeTail(HTlist), (int)NULL);

	DestroyList(HTlist);

	/*
	 * testing list concatenation
	 */

	int leader[] = {1, 2, 3, 4, 5};
	int trailer[] = {6, 7, 8, 9 , 10};

	List * leaderList = MakeIntListFromArr(leader, sizeof(leader) / sizeof(int));
	List * trailerList = MakeIntListFromArr(trailer, sizeof(trailer) / sizeof(int));

	List * wholeList = CatLists(leaderList, trailerList);
	shouldBe_Int(ListSize(wholeList), 10);
	for (int i = 0; i < 5; i++) {
		shouldBe_Int(ListContains(wholeList, &myIntCmp, (void *)&leader[i]), 1);
		shouldBe_Int(ListContains(wholeList, &myIntCmp, (void *)&trailer[i]), 1);
	}

	PrintList(wholeList, &printInt);
	DestroyList(wholeList);

	/* -- empty to empty -- */
	List * empty1 = NewList(&myIntDup);
	List * empty2 = NewList(&myIntDup);
	empty1 = CatLists(empty1, empty2);
	assert(empty1);
	shouldBe_Int(ListSize(empty1), 0);
	DestroyList(empty1);

	/* -- filled <- empty -- */
	int filled[] = {10,20,30,40,50,100};
	int dummy = 9999;

	List * filled1 = MakeIntListFromArr(filled, sizeof(filled) / sizeof(int));
	List * empty3 = NewList(&myIntDup);
	filled1 = CatLists(filled1, empty3);
	assert(filled1);
	shouldBe_Int(ListSize(filled1), 6);
	for (int i = 0; i < 6; i++) {
		shouldBe_Int(ListContains(filled1, &myIntCmp, (void *)&filled[i]), 1);
	}
	shouldBe_Int(AppendToList(filled1, (void *)&dummy), 7);
	PrintList(filled1, &printInt);
	DestroyList(filled1);

	/* -- empty <- filled -- */
	List * empty4 = NewList(&myIntDup);
	List * filled2 = MakeIntListFromArr(filled, sizeof(filled) / sizeof(int));
	filled2 = CatLists(empty4, filled2);
	assert(filled2);
	shouldBe_Int(ListSize(filled2), 6);
	for (int i = 0; i < 6; i++) {
		shouldBe_Int(ListContains(filled2, &myIntCmp, (void *)&filled[i]), 1);
	}
	shouldBe_Int(AppendToList(filled2, (void *)&dummy), 7);
	PrintList(filled2, &printInt);
	DestroyList(filled2);


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
