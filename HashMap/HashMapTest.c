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
		&hashPJW, 	// or %hashPJW
		&strIsEqual,
		&strIsEqual,
		5);
	assert(map);

	/*
	 * add a bunch of users and passwords
	 */
	char * users[] = {"Matt", "Andrew", "Sarah", "Janet", "Jim", "Martin"};
	char * passwords[] = {"purple", "blue", "red", "green", "yellow", "black"};
	for (int i = 0; i < 6; i++) {
		AddToHashMap(map, users[i], passwords[i]);
	}

	PrintHashMap(map, &printStrStrMap);
}
