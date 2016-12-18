#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Set.h"
#include "../HashTable/HashTable.h"
#include "../List/List.h"

Set * NewSet(CopyFunc copier, DestroyFunc destroyer, AreEqualFunc comparator) {
	if (!copier || !destroyer)
		return NULL;

	// use default hash function
	HashTable * set = NewHashTable(copier, destroyer, NULL, comparator, 3);
	assert(set);
	return (Set *)set;
}

void DestroySet(Set * s) {
	if (!s)
		return;
	HashTable * set = (HashTable *)s;
	DestroyHashTable(set);
}

int SetSize(Set * s) {
	if (!s)
		return -1;
	HashTable * set = (HashTable *)s;
	return HashTableSize(set);
}

int AddToSet(Set * s, void * element) {
	if (!s || !element)
		return 0;

	HashTable * set = (HashTable *)s;
	int added = 0;
	if (!HashTableContains(set, element)) {
		added = (AddToHashTable(set, element)) ? 1 : 0;
	}
	return added;
}

int SetContains(Set * s, void * element) {
	if (!s || !element)
		return 0;
	
	HashTable * set = (HashTable *)s;
	return HashTableContains(set, element);
}

void * RemoveFromSet(Set * s, void * element) {
	if (!s || !element)
		return NULL;
	HashTable * set = (HashTable *)s;
	return ExtractFromHashTable(set, element);
}

List * SetToList(Set * s) {
	if (!s)
		return NULL;
	HashTable * set = (HashTable *)s;
	return HashTableToList(set);
}

Set * FindIntersection(Set * s1, Set * s2) {
	if (!s1 || !s2)
		return NULL;

	HashTable * set1 = (HashTable *)s1;
	HashTable * set2 = (HashTable *)s2;

	if (CompareHashTableStructure(set1, set2) != 1) {
		fprintf(stderr,"Set Error -- sets have unequal structure\n");
		assert(0);
	}

	HashTable * intersection = CopyHashTableStructure(set1);
	if (!intersection)
		return NULL;

	// iterate over the smaller table
	HashTable * iteratedTable = NULL;
	HashTable * compareTable = NULL;
	if (set1 < set2) {
		iteratedTable = set1;
		compareTable = set2;
	} else {
		iteratedTable = set2;
		compareTable = set1;
	}

	HashTableIterator * i = NewHashTableIterator(iteratedTable);
	void * cur = GetHashTableIteratorCurrent(i);
	while (cur != NULL)	{
		if (HashTableContains(compareTable, cur) && !HashTableContains(intersection, cur)) {
			AddToHashTable(intersection, cur);
		}
		cur = AdvanceAndGetFromHashTableIterator(i);
	}
	DestroyHashTableIterator(i);

	return (Set *)intersection;
}

Set * FindUnion(Set * s1, Set * s2) {
	if (!s1 || !s2)
		return NULL;

	HashTable * set1 = (HashTable *)s1;
	HashTable * set2 = (HashTable *)s2;

	if (CompareHashTableStructure(set1, set2) != 1) {
		fprintf(stderr,"Set Error -- sets have unequal structure\n");
		assert(0);
	}

	HashTable * unionSet = CopyHashTableStructure(set1);
	if (!unionSet)
		return NULL;

	// iterate over set 1 and add unique entries
	HashTableIterator * i = NewHashTableIterator(set1);
	void * cur = GetHashTableIteratorCurrent(i);
	while (cur != NULL)	{
		if (!HashTableContains(unionSet, cur)) {
			AddToHashTable(unionSet, cur);
		}
		cur = AdvanceAndGetFromHashTableIterator(i);
	}
	DestroyHashTableIterator(i);

	// iterate over set 2 and add unique entries
	i = NewHashTableIterator(set2);
	cur = GetHashTableIteratorCurrent(i);
	while (cur != NULL)	{
		if (!HashTableContains(unionSet, cur)) {
			AddToHashTable(unionSet, cur);
		}
		cur = AdvanceAndGetFromHashTableIterator(i);
	}
	DestroyHashTableIterator(i);

	return (Set *)unionSet;
}

void PrintSet(Set * s, ApplyFunc printer) {
	if (!s || !printer)
		return;
	HashTable * set = (HashTable *)s;

	HashTableIterator * i = NewHashTableIterator(set);

	PrintHashTable(set, printer);

	void * cur = GetHashTableIteratorCurrent(i);

	printf("Set: {");
	while (cur) {
		printer(cur);
		cur = AdvanceAndGetFromHashTableIterator(i);
		if (cur) {
			printf(", ");
		}
	}
	printf("}\n");
	DestroyHashTableIterator(i);
}
