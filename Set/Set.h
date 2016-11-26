#ifndef SET_H
#define SET_H

typedef struct Set Set

#include "../HashTable/HashTable.h"

Set * NewSet(CopyInFunc copier, DestroyerFunc destroyer);
void DestroySet(Set set);

void * AddToSet(Set * set, void * element);

void * GetFromSet(Set * set, void * element);

void * RemoveFromSet(Set * set, void * element);

#endif // SET_H
