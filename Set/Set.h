#ifndef SET_H
#define SET_H

#include "../AbstractHelpers/AbstractFuncs.h"

typedef struct Set Set;

Set * NewSet(CopyFunc copier, DestroyFunc destroyer, AreEqualFunc comparator);
void DestroySet(Set * set);

/*
 * -- SetSize --
 * returns number of elements in set
 * returns -1 on error
 */
int SetSize(Set * set);

/*
 * -- AddToSet --
 * returns 1 if value is added
 * returns 0 if value already exists or error occurred
 */
int AddToSet(Set * set, void * element);

/*
 * -- SetContains --
 * returns 1 if value is present
 * returns 0 if value is not present or error occurred
 */
int SetContains(Set * set, void * element);

/*
 * -- RemoveFromSet --
 * returns extracted element, NULL if value isn't present
 *
 * Note: caller must claim returned memory
 */
void * RemoveFromSet(Set * set, void * element);

/*
 * -- MergeSets --
 * collect all unique entries between two sets
 * 
 * Usage: Set * newSet = MergeSets(set1, set2);
 */
Set * MergeSets(Set * set1, Set * set2);

void PrintSet(Set * s, ApplyFunc printer);

#endif // SET_H
