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

	HashTable * HashTable = NewHashTable(&myStrdup, &hashPJW, &strIsEqual, 10);
	int size = 0;
	shouldBe_Str(AddToHashTable(HashTable, "a"), "a"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "b"), "b"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "c"), "c"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "d"), "d"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "e"), "e"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "f"), "f"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "g"), "g"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "h"), "h"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "i"), "i"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "j"), "j"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "k"), "k"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "l"), "l"); size++;

	shouldBe_Str(AddToHashTable(HashTable, "double"), "double"); size++;
	shouldBe_Str(AddToHashTable(HashTable, "double"), "double"); size++;

	shouldBe_Int(HashTableSize(HashTable), size);
	PrintHashTable(HashTable, &printStr);

	shouldBe_Int(HashTableContains(HashTable, "a"), 1);
	shouldBe_Int(HashTableContains(HashTable, "z"), 0);
	shouldBe_Int(HashTableContains(HashTable, "i"), 1);
	shouldBe_Int(HashTableContains(HashTable, "not here"), 0);

	char * extracted = ExtractFromHashTable(HashTable, "a");
	shouldBe_Str(extracted, "a");
	free(extracted);

	shouldBe_Int(HashTableSize(HashTable), --size);

	shouldBe_Int(RemoveFromHashTable(HashTable, "double"), 2);
	size -= 2;
	shouldBe_Int(HashTableSize(HashTable), size);
	shouldBe_Int(HashTableContains(HashTable, "double"), 0);

	extracted = ExtractFromHashTable(HashTable, "a");
	shouldBe_Str(extracted, NULL);

	ClearHashTable(HashTable);
	shouldBe_Int(HashTableSize(HashTable), 0);
	PrintHashTable(HashTable, &printStr);

	DestroyHashTable(HashTable);


	printf("HashTable Tests Pass!\n");
	return 0;
}
