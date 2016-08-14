#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "TestingHelper.h"

void shouldBe_Int(int testingVal, int targetVal) {
	assert(testingVal == targetVal);
}

void shouldBe_Str(char * testingStr, char * targetStr) {
	assert(strcmp(testingStr, targetStr) == 0);
}

typedef struct _CustomTester {
	int (* IsEqualFunc)(void *, void *);
} _CustomTester;

CustomTester * MakeCustomTester(int (* IsEqualFunc)(void *, void *)) {
	if (!IsEqualFunc)
		return NULL;
	_CustomTester * ct = (_CustomTester *)calloc(1,sizeof(_CustomTester));
	assert(ct);
	ct->IsEqualFunc = IsEqualFunc;
	return (CustomTester *)ct;
}

void DestroyCustomTester(CustomTester * tester) {
	if (!tester)
		return;
	_CustomTester * ct = (_CustomTester *)tester;
	free(ct);
}

void shouldBe_Custom(CustomTester * tester, void * testArg, void * targetArg) {
	if (!tester)
		assert(0);
	_CustomTester * ct = (_CustomTester *)tester;
	assert(ct->IsEqualFunc(testArg, targetArg));
}

