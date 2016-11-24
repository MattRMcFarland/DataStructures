#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../TestingHelper/TestingHelper.h"
#include "../AbstractHelpers/StringHelper.h"
#include "HashMap.h"

void printStrStrMap(void * key, void * value) {
	char * k = (char *) key;
	char * v = (char *) value;
	printf(" {Key: %s, Value: %s} ", k, v);
}

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
		&hashPJW,
		&strIsEqual,
		&strIsEqual,
		10);
	assert(map);

}
