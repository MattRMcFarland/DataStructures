#ifndef TESTING_HELPER_H
#define TESTING_HELPER_H

void shouldBe_Int(int testingVal, int targetVal);
void shouldBe_IntPtr(int * testingVal, int targetVal);

void shouldBe_Str(char * testingStr, char * targetStr);

void shouldBe_NULL(void * testingVal);

void shouldBe_NonNULL(void * testingVal);

typedef struct CustomTester CustomTester;

CustomTester * MakeCustomTester(int (* IsEqualFunc)(void *, void *));
void DestroyCustomTester(CustomTester * tester);

void shouldBe_Custom(CustomTester * tester, void * testArg, void * targetArg);

#endif
