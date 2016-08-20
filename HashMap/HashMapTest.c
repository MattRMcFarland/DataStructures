#include <stdio.h>
#include <assert.h>
#include "../TestingHelper/TestingHelper.h"
#include "../AbstractHelpers/StringHelper.h"
#include "HashMap.h"

int main() {
	printf("\nRunning HashMap Tests...\n");

	/*
	 * test vanilla add, contains, extract, remove, size
	 */

	HashMap * hashmap = NewHashMap(&myStrdup, &hashPJW, &strIsEqual, 10);
	int size = 0;
	shouldBe_Str(AddToHashMap(hashmap, "a"), "a"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "b"), "b"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "c"), "c"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "d"), "d"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "e"), "e"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "f"), "f"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "g"), "g"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "h"), "h"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "i"), "i"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "j"), "j"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "k"), "k"); size++;
	shouldBe_Str(AddToHashMap(hashmap, "l"), "l"); size++;

	shouldBe_Int(HashMapSize(hashmap), size);
	PrintHashMap(hashmap, &printStr);

	DestroyHashMap(hashmap);


	printf("HashMap Tests Pass!\n");
	return 0;
}
