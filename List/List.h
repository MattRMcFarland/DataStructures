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

// returns new size of list, -1 if error occured. 
// cannot take NULL elements
int AppendToList(List * list, void * element);

void * GetFromList(List * list, ListSearchFunc, void * key);

// will remove *all* instances of `key`
void RemoveFromList(List * list, ListSearchFunc, void * key);

void ClearList(List * list);

void ListApply(List * list, ListApplyFunc toApply);

void PrintList(List * list, ListApplyFunc elementPrinter);

/* --- iterator --- */
typedef struct ListIterator ListIterator;

ListIterator * MakeListIterator(List * list);
void DestroyListIterator(ListIterator * iterator);

void * GetCurrentFromIterator(ListIterator * iterator);
void * AdvanceAndGetFromIterator(ListIterator * iterator);

#endif
