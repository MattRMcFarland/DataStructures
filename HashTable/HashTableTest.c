#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../TestingHelper/TestingHelper.h"
#include "../AbstractHelpers/StringHelper.h"
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
	 * test copy, toList, apply and iterator functions
	 */

	HashTable * copy = CopyHashTable(hashtable);
	printf("original --\n");
	PrintHashTable(hashtable, &printStr);
	printf("copy --\n");
	PrintHashTable(copy, &printStr);
	shouldBe_Int(HashTableSize(copy),HashTableSize(hashtable));

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
	 * clean up  
	 */

	ClearHashTable(hashtable);
	shouldBe_Int(HashTableSize(hashtable), 0);
	PrintHashTable(hashtable, &printStr);

	DestroyHashTable(hashtable);


	printf("HashTable Tests Pass!\n");
	return 0;
}
