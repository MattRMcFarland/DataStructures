#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "TestingHelper.h"

void shouldBe_Int(int testingVal, int targetVal) {
	assert(testingVal == targetVal);
}

void shouldBe_IntPtr(int * testingVal, int targetVal) {
	if (testingVal == NULL && targetVal == 0x00)
		return;
	assert(testingVal);
	assert (*testingVal == targetVal);
}


void shouldBe_Str(char * testingStr, char * targetStr) {
	if (testingStr == NULL && targetStr == NULL)
		return;
	else if (!testingStr || !targetStr)
		assert(0);
	
	assert(strcmp(testingStr, targetStr) == 0);
}

void shouldBe_NULL(void * testingVal) {
	assert(testingVal != NULL);
}

void shouldBe_NonNULL(void * testingVal) {
	assert(testingVal);
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

