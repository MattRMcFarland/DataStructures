#ifndef SET_H
#define SET_H

#include "../AbstractHelpers/AbstractFuncs.h"
#include "../List/List.h"

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
int SetContains(Set * set, void * key);

/*
 * -- RemoveFromSet --
 * returns extracted element, NULL if value isn't present
 *
 * Note: caller must claim returned memory
 */
void * RemoveFromSet(Set * set, void * key);

/*
 * -- SetToList --
 * copies the Set and turns it into a List
 * caller claims newly returned List structure
 *
 * returns List on success, NULL on failure
 */
List * SetToList(Set * set);

/*
 * -- FindIntersection --
 * collect all unique entries included in two sets and
 * creates a new set (does not claim either argument)
 *
 * Note: both sets must be structurally compatible,
 * 	(same copier, destroyer, and comparator functions)
 *
 * Note: caller claims returned set
 */
Set * FindIntersection(Set * set1, Set * set2);

/*
 * -- FindUnion --
 * creates a new set representing all unique entries 
 * represented in argument sets
 *
 * Note: both sets must be structurally compatible,
 * 	 (same copier, destroyer, and comparator functions)
 * 
 * Note: caller claims returned set
 */
Set * FindUnion(Set * set1, Set * set2);

void PrintSet(Set * s, ApplyFunc printer);

#endif // SET_H
