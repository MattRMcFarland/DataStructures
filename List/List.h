#ifndef LIST_H
#define LIST_H

#include "../AbstractHelpers/AbstractFuncs.h"

typedef struct List List;

List * NewList(CopyFunc copier, DestroyFunc destroyer);
void DestroyList(List * list);

// returns -1 on error
int ListSize(List * list);

// returns pointer to new element -- do not claim!
// cannot take NULL elements
void * AppendToList(List * list, void * element);

void * PutListHead(List * list, void * element);

/*
 * -- ExtractFromList --
 * removes data from list and returns reference to extracted element
 * returns NULL if not present or error occurs
 * 
 * Note: caller claims returned memory!
 * Note: key must be non-null
 */
void * ExtractFromList(List * list, AreEqualFunc f, void * key);

/*
 * -- RemoveFromList --
 * will remove all instances that match the given `key`
 * and returns number of removed elements (and 0 on error)
 *
 * Note: key must be non-null
 */
int RemoveFromList(List * list, AreEqualFunc f, void * key);

/*
 * -- ListContains --
 * returns 1 if element is present
 * returns 0 if not or error occurs
 *
 * Note: key must be non-null
 */
int ListContains(List * list, AreEqualFunc f, void * key);

/*
 * -- ListCount --
 * returns number of instances of element in List
 * returns 0 if not present or error
 *
 * Note: key must be non-null
 */
int ListCount(List * list, AreEqualFunc f, void * key);

// returns data element -- caller is responsible for returned memory reference
void * TakeHead(List * list);
void * TakeTail(List * list);

// returns help data element -- DO NOT FREE!
void * PeekHead(List * list);
void * PeekTail(List * list);

/* 
 * -- CatLists --
 * copies both argument lists and appends list2 to list1
 *
 * Note: caller must claim returned list reference!
 * 
 * Usage: List * catedList = CatList(list1, list2);
 */
List * CatLists(List * list1, List * list2);

// returns 1 if element 1 is less than element 2
// sorts lists in place from lowest to highest
typedef int (*CompareFunc)(const void * element1, const void * element2);

// and claims list argument and returns sorted list copy
// uses quicksort -- O(N * log(N))
List * SortList(List * list, CompareFunc comparator);

// returns reversed list, caller is reponsible for claiming original list
List * ReverseList(List * list);

void ClearList(List * list);
List * CopyList(List * list);
void ListApply(List * list, ApplyFunc toApply);
void PrintList(List * list, ApplyFunc elementPrinter);

/* --- iterator --- */
typedef struct ListIterator ListIterator;

// iterates over private contents -- do not claim!
ListIterator * MakeListIterator(List * list);
void DestroyListIterator(ListIterator * iterator);

void * GetCurrentFromIterator(ListIterator * iterator);
void * AdvanceAndGetFromIterator(ListIterator * iterator);

#endif
