#ifndef LIST_H
#define LIST_H

/*
 * This list structure is an abstracted List structure
 *
 * It holds void * references. When the list is created,
 * the user must pass in a reference to a "copy" function that
 * the list will use to create its own references to appended
 * objects. This list is responsible for all of its own memory.
 * Once an object is passed in, the user can free the reference
 * argument.
 *
 */


typedef void * (*CopyInFunc)(void * element);
typedef int (*ListSearchFunc)(void * element, void * key); // returns 1 on match
typedef void (*ListApplyFunc)(void * element);

typedef struct List List;

List * NewList(CopyInFunc copier);
void DestroyList(List * list);

// returns -1 on error
int ListSize(List * list);

// returns pointer to new element -- do not claim!
// cannot take NULL elements
void * AppendToList(List * list, void * element);
void * PutListHead(List * list, void * element);

// returns a reference of the first matching data element (do not free!)
void * GetFromList(List * list, ListSearchFunc, void * key);

// will remove *all* instances that match the given `key`
// returns number of removed elements, -1 on error
int RemoveFromList(List * list, ListSearchFunc, void * key);

// returns 1 if list contains an element that matches `key`
int ListContains(List * list, ListSearchFunc , void * key);

// returns data element -- caller is responsible for returned memory reference
void * TakeHead(List * list);
void * TakeTail(List * list);

// returns help data element -- do not free!
void * PeekHead(List * list);
void * PeekTail(List * list);

// claims list2!
List * CatLists(List * list1, List * list2);

void ClearList(List * list);
List * CopyList(List * list);
void ListApply(List * list, ListApplyFunc toApply);
void PrintList(List * list, ListApplyFunc elementPrinter);

/* --- iterator --- */
typedef struct ListIterator ListIterator;

ListIterator * MakeListIterator(List * list);
void DestroyListIterator(ListIterator * iterator);

void * GetCurrentFromIterator(ListIterator * iterator);
void * AdvanceAndGetFromIterator(ListIterator * iterator);

#endif
