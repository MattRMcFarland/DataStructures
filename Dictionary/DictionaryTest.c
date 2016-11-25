#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../TestingHelper/TestingHelper.h" 
#include "../AbstractHelpers/IntHelper.h" 
#include "../AbstractHelpers/StringHelper.h"
#include "Dictionary.h"

int main() {
	printf("\nRunning Dictionary Tests\n");

	/*
	 * Test an Int dictionary
	 */

	Dictionary * dictionary = NewDictionary(
		&myIntDup,
		&safeFree,
		&myIntIsEqual,
		3);

	char * users[] = {"Matt", "Andrew", "Sarah", "Janet", "Jim", "Martin", "Alex", "Josh"};
	int ages[] = {22, 19, 22, 53, 55, 18, 16, 12};
	int n = sizeof(users) / sizeof(char *);

	int size = 0;
	shouldBe_Int(GetDictionarySize(dictionary), size);
	for (int i = 0; i < n; i++) {
		shouldBe_Int(IsDefined(dictionary, users[i]), 0);

		int * added = (int *)AddDefinition(dictionary, users[i], &ages[i]);
		shouldBe_IntPtr(added, ages[i]);

		printf("user: %s, age: %d\n", users[i], *(int *)Lookup(dictionary, users[i]));
		shouldBe_IntPtr((int *)Lookup(dictionary, users[i]), ages[i]);

		size++;
		shouldBe_Int(GetDictionarySize(dictionary), size);

		shouldBe_Int(IsDefined(dictionary, users[i]), 1);
	}
	shouldBe_IntPtr((int *)Lookup(dictionary, "Not There"), 0x00);
	PrintDictionary(dictionary, &printInt);


}
