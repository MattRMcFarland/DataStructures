#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../TestingHelper/TestingHelper.h"
#include "../AbstractHelpers/StringHelper.h"
#include "../AbstractHelpers/IntHelper.h"
#include "HashTable.h"

int main() {
	printf("\nRunning HashTable Tests...\n");

	/*
	 * test vanilla add, contains, extract, remove, size
	 */

	HashTable * hashtable = NewHashTable(&myStrdup, &safeFree, &hashPJW, &strIsEqual, 10);
	int size = 0;
	shouldBe_Str(AddToHashTable(hashtable, "aa"), "aa"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "bb"), "bb"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "cc"), "cc"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "dd"), "dd"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "ee"), "ee"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "ff"), "ff"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "gg"), "gg"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "hh"), "hh"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "ii"), "ii"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "jj"), "jj"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "kk"), "kk"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "ll"), "ll"); size++;

	shouldBe_Str(AddToHashTable(hashtable, "double"), "double"); size++;
	shouldBe_Str(AddToHashTable(hashtable, "double"), "double"); size++;

	shouldBe_Int(HashTableSize(hashtable), size);
	PrintHashTable(hashtable, &printStr);

	shouldBe_Int(HashTableContains(hashtable, "aa"), 1);
	shouldBe_Int(HashTableContains(hashtable, "zz"), 0);
	shouldBe_Int(HashTableContains(hashtable, "ii"), 1);
	shouldBe_Int(HashTableContains(hashtable, "not here"), 0);

	shouldBe_Int(HashTableContains(NULL, "aa"), 0);
	shouldBe_Int(HashTableContains(hashtable, NULL), 0);

	char * extracted = ExtractFromHashTable(hashtable, "aa");
	shouldBe_Str(extracted, "aa");
	free(extracted);

	shouldBe_Int(HashTableSize(hashtable), --size);

	shouldBe_Int(RemoveFromHashTable(hashtable, "double"), 2);
	size -= 2;
	shouldBe_Int(HashTableSize(hashtable), size);
	shouldBe_Int(HashTableContains(hashtable, "double"), 0);

	extracted = ExtractFromHashTable(hashtable, "aa");
	shouldBe_Str(extracted, NULL);

	extracted = ExtractFromHashTable(hashtable, "ff");
	shouldBe_Str(extracted, "ff"); 
	free(extracted);

	/* 
	 * test copy, structural copy, toList, apply and iterator functions
	 */

	HashTable * copy = CopyHashTable(hashtable);
	HashTable * structuralCopy = CopyHashTableStructure(hashtable);

	printf("original --\n");
	PrintHashTable(hashtable, &printStr);

	printf("copy --\n");
	PrintHashTable(copy, &printStr);
	shouldBe_Int(HashTableSize(copy),HashTableSize(hashtable));

	printf("structural copy --\n");
	shouldBe_Int(CompareHashTableStructure(hashtable, structuralCopy), 1);
	shouldBe_Int(HashTableSize(structuralCopy), 0);
	AddToHashTable(structuralCopy, "should stick!");
	PrintHashTable(structuralCopy, &printStr);
	DestroyHashTable(structuralCopy);

	List * contents = HashTableToList(hashtable);
	shouldBe_Int(ListSize(contents), HashTableSize(hashtable));

	ListIterator * contentsIterator = MakeListIterator(contents);
	void * contentsProbe = GetCurrentFromIterator(contentsIterator);

	HashTableIterator * iterator = NewHashTableIterator(hashtable);
	void * probe = GetHashTableIteratorCurrent(iterator);

	int iterations = 0;
	while (probe) {
		iterations++;
		shouldBe_Str((char*)probe, (char*)contentsProbe);
		shouldBe_Int(HashTableContains(hashtable, probe), 1);
		shouldBe_Int(HashTableContains(copy, probe), 1);
		contentsProbe = AdvanceAndGetFromIterator(contentsIterator);
		probe = AdvanceAndGetFromHashTableIterator(iterator);
	}
	shouldBe_Int(iterations, HashTableSize(hashtable));

	printf("should be squashed...\n");
	HashTable * squashed = ApplyToHashTable(copy, &squasher);
	PrintHashTable(squashed, &printStr);

	DestroyHashTableIterator(iterator);
	DestroyListIterator(contentsIterator);
	DestroyList(contents);
	DestroyHashTable(copy);

	/*
	 * test structural comparison and copying
	 */

	HashTable * strTable = NewHashTable(&myStrdup, &safeFree, &hashPJW, &strIsEqual, 10);
	HashTable * strTable2 = NewHashTable(&myStrdup, &safeFree, &hashPJW, &strIsEqual, 10);

	HashTable * intTable1 = NewHashTable(&myIntDup, &safeFree, &hashPJW, &strIsEqual, 10);
	HashTable * intTable2 = NewHashTable(&myStrdup, &free, &hashPJW, &strIsEqual, 10);
	HashTable * intTable3 = NewHashTable(&myStrdup, &safeFree, &hashPJW, &myIntCompare, 10);

	shouldBe_Int(CompareHashTableStructure(strTable, strTable2), 1);
	shouldBe_Int(CompareHashTableStructure(strTable, intTable1), 0);
	shouldBe_Int(CompareHashTableStructure(strTable, intTable2), 0);
	shouldBe_Int(CompareHashTableStructure(strTable, intTable3), 0);

	DestroyHashTable(strTable);
	DestroyHashTable(strTable2);
	DestroyHashTable(intTable1);
	DestroyHashTable(intTable2);
	DestroyHashTable(intTable3);

	/*
	 * clean up  
	 */

	ClearHashTable(hashtable);
	shouldBe_Int(HashTableSize(hashtable), 0);
	printf("should be empty --\n");
	PrintHashTable(hashtable, &printStr);

	DestroyHashTable(hashtable);

	printf("HashTable Tests Pass!\n");
	return 0;
}
