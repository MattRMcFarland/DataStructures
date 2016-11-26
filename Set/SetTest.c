#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../AbstractHelpers/StringHelper.h"
#include "../TestingHelper/TestingHelper.h"
#include "Set.h"

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
	shouldBe_Int(SetContains(set, NULL), 0);
	shouldBe_Int(SetContains(NULL, "NotThere"), 0);

	PrintSet(set, &printStr);

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
}
