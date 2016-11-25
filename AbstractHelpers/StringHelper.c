#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "StringHelper.h"


/* Peter Weinberger's hash function, from Aho, Sethi, & Ullman
   p. 436. */
unsigned hashPJW(void * string) {
  unsigned h = 0, g;
  char *p;

  for (p = string; *p != '\0'; p++) {
    h = (h << 4) + *p;
    if ((g = (h & 0xf0000000)) != 0)
  		h ^= (g >> 24) ^ g;
  }

  return h;
}

void * myStrdup(void * string) {
	return (void *)strdup((char *)string);
}

void safeFree(void * string) {
	if (string)
		free(string);
}

int strIsEqual(void * testStr, void * targetStr) {
	char * testS = (char *)testStr;
	char * targetS = (char *)targetStr;

	if (testS == NULL && targetS == NULL)
		return 1;
	else if (!testS || !targetS)
		return 0;

	return (strcmp(testS, targetS) == 0) ? 1 : 0;
}

void printStr(void * str) {
	if (!str)
		return;
	printf("%s", (char *)str);
}

void printStrStrMap(void * key, void * value) {
	char * k = (char *) key;
	char * v = (char *) value;
	printf(" {Key: %s, Value: %s} ", k, v);
}

void squasher(void * string) {
	if (!string)
		return;
	char * s = (char *)string;
	if (strlen(s) > 0)
		s[0] = '_';
}

void squashStrStr(void * k, void * v) {
	squasher(k);
	squasher(v);
}
