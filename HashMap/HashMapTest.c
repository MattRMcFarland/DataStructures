#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../TestingHelper/TestingHelper.h"
#include "../AbstractHelpers/StringHelper.h"
#include "HashMap.h"

int main() {
	printf("\nRunning HashMap Tests...\n");

	/*
	 * Test a String - String map
	 */ 

	HashMap * map = NewHashMap(
		&myStrdup, 
		&myStrdup, 
		&safeFree,
		&safeFree,
		&hashPJW, 	// or %hashPJW
		&strIsEqual,
		&strIsEqual,
		5);
	assert(map);

	/*
	 * add a bunch of users and passwords
	 */
	char * users[] = {"Matt", "Andrew", "Sarah", "Janet", "Jim", "Martin", "Alex", "Josh"};
	char * passwords[] = {"purple", "blue", "red", "green", "yellow", "black", "cyan", "pink"};
	int n = sizeof(users) / sizeof(char *);

	int size = 0;
	shouldBe_Int(GetHashMapSize(map), size);
	for (int i = 0; i < n; i++) {
		shouldBe_Int(ContainsKey(map, users[i]), 0);
		char * added = (char *)AddToHashMap(map, users[i], passwords[i]);

		shouldBe_Str(added, passwords[i]);
		printf("key: %s, value: %s\n", users[i], (char *)GetValueFromHashMap(map, users[i]));
		shouldBe_Str((char *)GetValueFromHashMap(map, users[i]), passwords[i]);

		size++;
		shouldBe_Int(GetHashMapSize(map), size);

		shouldBe_Int(ContainsKey(map, users[i]), 1);
	}
	shouldBe_Str((char *)GetValueFromHashMap(map, "Not There"), NULL);
	PrintHashMap(map, &printStr, &printStr);

	/*
	 * Test error cases for addition and retrieval of keys
	 */

	shouldBe_Int(GetHashMapSize(NULL), -1);

	shouldBe_Str((char *)AddToHashMap(map, "null", NULL), NULL);
	shouldBe_Str((char *)AddToHashMap(map,  NULL, "null"), NULL);

	shouldBe_Str((char *)GetValueFromHashMap(NULL, "Not There"), NULL);
	shouldBe_Str((char *)GetValueFromHashMap(map, NULL), NULL);

	/*
	 * Test an update
	 */
	shouldBe_Str((char *)AddToHashMap(map, "Matt", "NewColor"), "NewColor");
	shouldBe_Str((char *)GetValueFromHashMap(map, "Matt"), "NewColor");
	shouldBe_Int(GetHashMapSize(map), size);
	PrintHashMap(map, &printStr, &printStr);

	/* 
	 * Test value extraction
	 */
	shouldBe_Str((char *)ExtractFromHashMap(map, "Not There"), NULL);
	char * extracted = (char *)ExtractFromHashMap(map, "Andrew");
	shouldBe_Str(extracted, "blue");
	free(extracted);
	shouldBe_Int(GetHashMapSize(map), --size);
	shouldBe_Int(ContainsKey(map, "Andrew"), 0);
	shouldBe_Str((char *)GetValueFromHashMap(map, "Andrew"), NULL);
	PrintHashMap(map, &printStr, &printStr);

	/* 
	 * Test HashMap Iteration
	 */

	int expected = size;
	HashMapIterator * iterator = NewHashMapIterator(map);
	char * curKey = NULL;
	char * curValue = NULL;

	GetHashMapIteratorCurrent(iterator, (void **)&curKey, (void **)&curValue);
	while (curKey != NULL && curValue != NULL) {
		expected--;
		shouldBe_Str((char *)GetValueFromHashMap(map, curKey), curValue);
		AdvanceAndGetFromHashMapIterator(iterator, (void **)&curKey, (void **)&curValue);
		if (expected < 0)
			shouldBe_Int(expected, 0);
	}
	shouldBe_Int(expected, 0);
	DestroyHashMapIterator(iterator);

	/*
	 * Test HashMap Apply
	 */
	HashMapApply(map, &squashStrStr);
	PrintHashMap(map, &printStr, &printStr);

	DestroyHashMap(map);
}
