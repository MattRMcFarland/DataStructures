#ifndef LIST_H
#define LIST_H

typedef void * (*CopyInFunc)(void * element);
typedef int (*ListSearchFunc)(void * element, void * key);
typedef void (*ListApplyFunc)(void * element);

typedef struct List List;

List * NewList(CopyInFunc copier);
void DestroyList();

// returns -1 on error
int ListSize(List * list);

// returns new size of list, -1 if error occured. 
int AppendToList(List * list, void * element);

void * RemoveFromList(List * list, ListSearchFunc, void * key);
void * GetFromList(List * list, ListSearchFunc, void * key);
void ListApply(List * list, ListApplyFunc * toApply);
void ClearList(List * list);

void PrintList(List * list);

#endif
