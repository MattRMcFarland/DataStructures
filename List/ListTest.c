#include <stdio.h>
#include "List.h"
#include "../TestingHelper/TestingHelper.h"

int main() {
	printf("\nRunning List Test...\n");

	/*
	 * vanilla append and contains tests
	 */

	//StringList * list = lNew();

	// lPrint(list);

	// char forLater[] = "find me!";

	// int appendTests = 5;
	// char * testVals[] = {"First!", "Second!", "Third!", "Fourth!", "Fifth!"};

	// shouldBe_Int(lSize(list), 0);

	// for (int i = 0; i < appendTests; i++) {
	// 	shouldBe_Int(lAppend(list, testVals[i]), i + 1);
	// }

	// shouldBe_Int(lAppend(NULL, "blah"), -1);
	// shouldBe_Int(lAppend(list, NULL), -1);

	// shouldBe_Int(lAppend(list, forLater), 6);
	// shouldBe_Int(lContains(list, forLater), 1);
	// shouldBe_Int(lContains(list, "not here"), 0);

	// shouldBe_Int(lSize(list), 6);

	// lPrint(list);

	// /* 
	//  * compare lists
	//  */

	// StringList * list2 = lNew();
	// for (int i = 0; i < appendTests; i++) {
	// 	shouldBe_Int(lAppend(list2, testVals[i]), i + 1);
	// }

	// shouldBe_Int(lAreEqual(list, list2), 0);

	// shouldBe_Int(lAppend(list2, forLater), 6);

	// CustomTester * listTester = MakeCustomTester(&lAreEqual);
	// shouldBe_Custom(listTester, list, list2);
	// DestroyCustomTester(listTester);
	// lDestroy(list2);

	// /*
	//  * iterator tests
	//  */

	// StringListIterator * iterator = lNewIterator(list);

	// for (int i = 0; i < appendTests; i++) {
	// 	shouldBe_Str(iGet(iterator),testVals[i]);
	// 	shouldBe_Int(iHasNext(iterator), 1);
	// 	iAdvance(iterator);
	// }

	// shouldBe_Str(iGet(iterator), forLater);
	// shouldBe_Int(iHasNext(iterator), 0);

	// lDestroyIterator(iterator);
	// lDestroy(list);

	// printf("StringList Tests pass!\n\n");

	return 0;
}
