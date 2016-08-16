#include <stdio.h>
#include <string.h>
#include "List.h"
#include "../TestingHelper/TestingHelper.h"

void * myStrdup(void * element) {
	return (void *)strdup((char *)element);
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

	shouldBe_Str(GetFromList(list, &strIsEqual, "d"), NULL);
	shouldBe_Str(GetFromList(list, &strIsEqual, "e"), NULL);
	shouldBe_Str(GetFromList(list, &strIsEqual, "a"), NULL);

	ClearList(list);
	shouldBe_Int(ListSize(list), 0);

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
