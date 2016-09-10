#ifndef INT_HELPER_H
#define INT_HELPER_H

#include "../List/List.h"

void * myIntDup(void * integer);
int myIntIsEqual(void * testInteger, void * targetInteger);

// NULL is considered LEAST value possible
int myIntCompare(const void * arg1, const void * arg2);

void printInt(void * integer);
void printIntList(List * list);

void incInt(void * integer);

#endif // INT_HELPER_H
