#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Set.h"
#include "../HashTable/HashTable.h"

Set * NewSet(CopyFunc copier, DestroyFunc destroyer, AreEqualFunc comparator) {
	if (!copier || !destroyer)
		return NULL;

	// use default hash function
	HashTable * set = NewHashTable(copier, destroyer, NULL, comparator, 100);
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

void PrintSet(Set * s, ApplyFunc printer) {
	if (!s || !printer)
		return;
	HashTable * set = (HashTable *)s;

	HashTableIterator * i = NewHashTableIterator(set);
	void * cur = GetHashTableIteratorCurrent(i);

	printf("Set {");
	while (cur) {
		printer(cur);
		printf(", ");
		cur = AdvanceAndGetFromHashTableIterator(i);
	}
	printf("}\n");
	DestroyHashTableIterator(i);
}
