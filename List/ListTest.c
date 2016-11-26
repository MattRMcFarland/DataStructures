#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "List.h"
#include "../TestingHelper/TestingHelper.h"
#include "../AbstractHelpers/StringHelper.h"
#include "../AbstractHelpers/IntHelper.h"

/* --- additional integer list helpers --- */

List * MakeIntListFromArr(int arr[], int size) {
	if (!arr || size < 0)
		return NULL;

	List * new = NewList(&myIntDup, &safeFree);
	for (int i = 0; i < size; i++) {
		AppendToList(new,(void *)&arr[i]);
	}
	return new;
}

int intArrCmp(const void * a1, const void * a2) {
	return (*(int *)a1 - *(int *)a2);
}

/* 
 * ##							 ##
 * ### test suite ###
 * ##							 ##
 */

int main() {
	printf("\nRunning List Test...\n");

	/*
	 * size, put, append, contains, remove, copy tests
	 */

	List * list = NewList(&myStrdup, &safeFree);
	List * emptyCopy = CopyList(list);

	shouldBe_Int(ListSize(list), 0);
	shouldBe_Int(ListSize(emptyCopy), 0);
	int size = 0;

	shouldBe_Str(AppendToList(list, "a"), "a");
	shouldBe_Str(PutListHead(list, "m"), "m");
	shouldBe_Str(AppendToList(list, "d"), "d");
	shouldBe_Str(AppendToList(list, "b"), "b");
	shouldBe_Str(AppendToList(list, "c"), "c");
	shouldBe_Str(AppendToList(list, "d"), "d");
	shouldBe_Str(PutListHead(list, "n"), "n");
	shouldBe_Str(AppendToList(list, "d"), "d");
	shouldBe_Str(AppendToList(list, "e"), "e");
	shouldBe_Int(size = ListSize(list), 9);

	List * fullCopy = CopyList(list);
	shouldBe_Int(ListSize(fullCopy), size);
	PrintList(fullCopy, printStr);
	DestroyList(emptyCopy);
	DestroyList(fullCopy);

	shouldBe_Str(AppendToList(list, NULL), NULL);
	shouldBe_Str(AppendToList(NULL, "a"), NULL);

	PrintList(list, printStr);

	shouldBe_Int(ListContains(list, &strIsEqual, "c"), 1);
	shouldBe_Int(ListContains(list, &strIsEqual, "z"), 0);

	shouldBe_Int(ListCount(list, &strIsEqual, "a"), 1);
	shouldBe_Int(ListCount(list, &strIsEqual, "d"), 3);
	shouldBe_Str((char *)ExtractFromList(list, &strIsEqual, "not here"), NULL);

	char * extracted = (char *)ExtractFromList(list, &strIsEqual, "d");
	shouldBe_Str(extracted, "d");
	free(extracted);

	shouldBe_Int(ListSize(list),--size);
	shouldBe_Int(ListCount(list, &strIsEqual, "d"), 2);

	shouldBe_Int(RemoveFromList(list, &strIsEqual, "a"), 1);
	shouldBe_Int(ListSize(list), --size);
	PrintList(list, printStr);

	shouldBe_Int(RemoveFromList(list, &strIsEqual, "e"), 1);
	shouldBe_Int(ListSize(list), --size);
	PrintList(list, printStr);

	shouldBe_Int(RemoveFromList(list, &strIsEqual, "d"), 2);
	size = size - 2;
	shouldBe_Int(ListSize(list), size);
	PrintList(list, printStr);

	shouldBe_Int(RemoveFromList(list, &strIsEqual, "not here"), 0);

	shouldBe_Int(ListContains(list, &strIsEqual, "d"), 0);
	shouldBe_Int(ListContains(list, &strIsEqual, "e"), 0);
	shouldBe_Int(ListContains(list, &strIsEqual, "a"), 0);

	shouldBe_Int(ListContains(list, &strIsEqual, "c"), 1);
	shouldBe_Int(ListContains(list, &strIsEqual, "a"), 0);

	ClearList(list);
	shouldBe_Int(ListSize(list), 0);

	/*
	 * testing head and tail functions
	 */

	int arr[] = {1, 3, 5, 7, 9, 11};
	int HTsize = sizeof(arr) / sizeof(int);

	List * HTlist = NewList(&myIntDup, &safeFree);
	for (int i = 0; i < HTsize; i++) {
		shouldBe_Int(*(int *)AppendToList(HTlist, &arr[i]), arr[i]);
	}
	shouldBe_Int(ListSize(HTlist), HTsize);
	printIntList(HTlist);

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

	printIntList(HTlist);

	ListApply(HTlist, &incInt);
	printIntList(HTlist);

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
		shouldBe_Int(ListContains(wholeList, &myIntIsEqual, (void *)&leader[i]), 1);
		shouldBe_Int(ListContains(wholeList, &myIntIsEqual, (void *)&trailer[i]), 1);
	}

	printIntList(wholeList);
	DestroyList(wholeList);

	/* -- empty to empty -- */
	List * empty1 = NewList(&myIntDup, &safeFree);
	List * empty2 = NewList(&myIntDup, &safeFree);
	empty1 = CatLists(empty1, empty2);
	assert(empty1);
	shouldBe_Int(ListSize(empty1), 0);
	DestroyList(empty1);

	/* -- filled <- empty -- */
	int filled[] = {10,20,30,40,50,100};
	int dummy = 9999;

	List * filled1 = MakeIntListFromArr(filled, sizeof(filled) / sizeof(int));
	List * empty3 = NewList(&myIntDup, &safeFree);
	filled1 = CatLists(filled1, empty3);
	assert(filled1);
	shouldBe_Int(ListSize(filled1), 6);
	for (int i = 0; i < 6; i++) {
		shouldBe_Int(ListContains(filled1, &myIntIsEqual, (void *)&filled[i]), 1);
	}
	shouldBe_Int(*(int *)AppendToList(filled1, (void *)&dummy), dummy);
	shouldBe_Int(ListSize(filled1), 7);
	printIntList(filled1);
	DestroyList(filled1);

	/* -- empty <- filled -- */
	List * empty4 = NewList(&myIntDup, &safeFree);
	List * filled2 = MakeIntListFromArr(filled, sizeof(filled) / sizeof(int));
	filled2 = CatLists(empty4, filled2);
	assert(filled2);
	shouldBe_Int(ListSize(filled2), 6);
	for (int i = 0; i < 6; i++) {
		shouldBe_Int(ListContains(filled2, &myIntIsEqual, (void *)&filled[i]), 1);
	}
	shouldBe_Int(*(int *)AppendToList(filled2, (void *)&dummy), dummy);
	shouldBe_Int(ListSize(filled2), 7);
	printIntList(filled2);
	DestroyList(filled2);


	/*
	 * testing apply and reverse
	 */

	AppendToList(list, "cat");
	AppendToList(list, "dog");
	AppendToList(list, "hamster");
	AppendToList(list, "cheetah");
	PrintList(list, &printStr);

	List * reversed = ReverseList(list);
	PrintList(reversed, &printStr);
	shouldBe_Int(ListSize(reversed), ListSize(list));
	shouldBe_Str(PeekHead(reversed), PeekTail(list));
	shouldBe_Str(PeekHead(list), PeekTail(reversed));
	DestroyList(reversed);

	ListApply(list, &squasher);
	PrintList(list, &printStr);

	/*
	 * testing sort
	 */

	int nums[] = {-10, 20, 50, 2, 89, 4, 2, -1, 0, 9000, 1000, 50};
	int numsLen = sizeof(nums) / sizeof(int);
	List * unsorted = MakeIntListFromArr(nums, numsLen);
	printf("unsorted list: ");
	printIntList(unsorted);

	qsort(nums, numsLen, sizeof(int), &intArrCmp);

	List * sorted = SortList(unsorted, &myIntCompare);
	printf("sorted list: ");
	printIntList(sorted);

	ListIterator * sortedIterator = MakeListIterator(sorted);
	int sortedIterations = 0;
	void * sortedCurrent = GetCurrentFromIterator(sortedIterator);
	while (sortedCurrent) {
		shouldBe_Int(*(int *)sortedCurrent, nums[sortedIterations++]);
		sortedCurrent = AdvanceAndGetFromIterator(sortedIterator);
	}

	DestroyListIterator(sortedIterator);
	DestroyList(sorted);

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
