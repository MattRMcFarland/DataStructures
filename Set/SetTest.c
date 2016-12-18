#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../AbstractHelpers/StringHelper.h"
#include "../AbstractHelpers/IntHelper.h"
#include "../TestingHelper/TestingHelper.h"
#include "Set.h"
#include "../List/List.h"

int CompareSets(Set * set1, Set * set2) {
	if (!set1 || !set2)
		return 0;

	int size1 = SetSize(set1);
	if (size1 != SetSize(set2))
		return 0;

	Set * intersection = FindIntersection(set1, set2);
	int isSame = (SetSize(intersection) == size1) ? 1 : 0;
	DestroySet(intersection);

	return isSame;
}

int main() {
	printf("\nRunning Set Tests...\n");

	/* 
	 * Create a string set
	 */
	Set * set = NewSet(&myStrdup, &safeFree, &strIsEqual);
	assert(set);

	char * users[] = {"Matt", "Sarah", "Andrew", "Audrey", "Daniel", "Ian"};
	int n = sizeof(users) / sizeof(char *);

	int size = 0;
	shouldBe_Int(SetSize(set), size);
	for (int i = 0; i < n; i++) {
		shouldBe_Int(AddToSet(set, users[i]), 1);
		size++;
		shouldBe_Int(SetContains(set, users[i]), 1);
	}

	shouldBe_Int(SetContains(set, "NotThere"), 0);

	// test edge cases
	shouldBe_Int(SetContains(set, NULL), 0);
	shouldBe_Int(SetContains(NULL, "NotThere"), 0);

	PrintSet(set, &printStr);

	/*
	 * Test SetToList
	 */
	List * list = SetToList(set);
	assert(list);
	shouldBe_Int(size, ListSize(list));

	printf("List: ");
	PrintList(list, &printStr);

	ListIterator * li = MakeListIterator(list);
	void * curListEntry = GetCurrentFromIterator(li);
	while (curListEntry) {
		shouldBe_Int(SetContains(set, curListEntry), 1);
		curListEntry = AdvanceAndGetFromIterator(li);
	}
	DestroyListIterator(li);

	shouldBe_Int(ListContains(list, &strIsEqual, "NotThere"), 0);

	DestroyList(list);

	/*
	 * Test Union and Intersection functions
	 */

	// try edge cases
	shouldBe_NULL(FindIntersection(NULL, NULL));
	shouldBe_NULL(FindIntersection(NULL, set));
	shouldBe_NULL(FindIntersection(set, NULL));

	shouldBe_NULL(FindUnion(NULL, NULL));
	shouldBe_NULL(FindUnion(NULL, set));
	shouldBe_NULL(FindUnion(set, NULL));

	// try with mismatched structures  
	// -- don't actually need b/c if set structures are different, program halts
	// 
	// Set * intSet1 = NewSet(&myIntDup, &safeFree, &strIsEqual);
	// Set * intSet2 = NewSet(&myStrdup, &free, &strIsEqual);
	// Set * intSet3 = NewSet(&myStrdup, &safeFree, &myIntCompare);

	// shouldBe_NULL(FindIntersection(intSet1, set));
	// shouldBe_NULL(FindIntersection(intSet2, set));
	// shouldBe_NULL(FindIntersection(intSet3, set));

	// shouldBe_NULL(FindUnion(intSet1, set));
	// shouldBe_NULL(FindUnion(intSet2, set));
	// shouldBe_NULL(FindUnion(intSet3, set));

	// DestroySet(intSet1);
	// DestroySet(intSet2);
	// DestroySet(intSet3);

	// try with empty Set
	Set * emptySet = NewSet(&myStrdup, &safeFree, &strIsEqual);

	Set * intersection1 = FindIntersection(emptySet, set);
	shouldBe_NonNULL(intersection1);
	shouldBe_Int(SetSize(intersection1), 0);

	Set * union1 = FindUnion(emptySet, set);
	shouldBe_NonNULL(union1);
	shouldBe_Int(SetSize(union1), SetSize(set));

	DestroySet(intersection1);
	DestroySet(union1);

	// try with partial sets now
	Set * partialUsers = NewSet(&myStrdup, &safeFree, &strIsEqual);
	int partialN = 3;
	for (int i = 0; i < partialN; i++) {
		AddToSet(partialUsers, users[i]);
	}

	Set * intersection = FindIntersection(partialUsers, set);
	shouldBe_NonNULL(intersection);
	shouldBe_Int(SetSize(intersection), partialN);

	for (int i = 0; i < n; i++) {
		if (i < partialN) {
			shouldBe_Int(SetContains(partialUsers, users[i]), 1);
		} else {
			shouldBe_Int(SetContains(partialUsers, users[i]), 0);
		}
	}

	DestroySet(intersection);
	DestroySet(partialUsers);

	// add the Avengers!
	char * avengerNames[] = {"Ironman", "Hulk", "Black Widow", "Thor", "Captain America"};
	int avengersN = sizeof(avengerNames) / sizeof(char *);

	Set * avengers = NewSet(&myStrdup, &safeFree, &strIsEqual);
	for (int i = 0; i < avengersN; i++) {
		AddToSet(avengers, avengerNames[i]);
	}

	Set * avengersIntersection = FindIntersection(avengers, set);
	shouldBe_NonNULL(avengersIntersection);
	shouldBe_Int(SetSize(avengersIntersection), 0);
	DestroySet(avengersIntersection);

	Set * bigUnion = FindUnion(avengers, set);
	shouldBe_NonNULL(bigUnion);
	shouldBe_Int(SetSize(bigUnion), SetSize(avengers) + SetSize(set));

	PrintSet(bigUnion, &printStr);

	List * bigUnionList = SetToList(bigUnion);
	shouldBe_NonNULL(bigUnionList);
	ListIterator * bigULI = MakeListIterator(bigUnionList);
	char * currentUserAvenger = GetCurrentFromIterator(bigULI);
	while (currentUserAvenger) {
		shouldBe_Int(SetContains(avengers, currentUserAvenger) || SetContains(set, currentUserAvenger), 1);
		currentUserAvenger = AdvanceAndGetFromIterator(bigULI);
	}
	DestroyListIterator(bigULI);

	Set * avengers2 = FindIntersection(avengers, bigUnion);
	shouldBe_Int(CompareSets(avengers2, avengers), 1);
	DestroySet(avengers2);

	Set * set2 = FindIntersection(set, bigUnion);
	shouldBe_Int(CompareSets(set2, set), 1);
	DestroySet(set2);

	DestroySet(bigUnion);

	/*
	 * Test Removal
	 */
	shouldBe_Str((char *)RemoveFromSet(set, "NotThere"), NULL);
	char * removed = RemoveFromSet(set, "Matt");
	shouldBe_Str(removed, "Matt");
	free(removed);
	shouldBe_Int(SetSize(set), --size);

	PrintSet(set, &printStr);

	/*
	 * clean up
	 */
	DestroySet(set);

	printf("Set Tests Success!\n");
	return 0;
}
