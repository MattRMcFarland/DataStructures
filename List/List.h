#ifndef LIST_H
#define LIST_H

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

// removes data from list and returns reference, caller claims returned memory!
void * ExtractFromList(List * list, ListSearchFunc, void * key);

// will remove *all* instances that match the given `key`
// returns number of removed elements, -1 on error
int RemoveFromList(List * list, ListSearchFunc, void * key);

// returns 1 if list contains an element that matches `key`, 0 if absent, -1 on error
int ListContains(List * list, ListSearchFunc , void * key);

// returns count of elements matching `key`, -1 on error
int ListCount(List * list, ListSearchFunc, void * key);

// returns data element -- caller is responsible for returned memory reference
void * TakeHead(List * list);
void * TakeTail(List * list);

// returns help data element -- DO NOT FREE!
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
