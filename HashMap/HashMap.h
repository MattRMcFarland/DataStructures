#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMap HashMap;

typedef void * (*CopyFunc)(void * element);
typedef int (*HashFunc)(void * element);
typedef int (*AreEqualFunc)(void * element1, void * element2);
typedef int (*HashMapApplyFunc)(void * element);

HashMap * NewHashMap(CopyFunc cf, HashFunc hf, AreEqualFunc aef, int total);
void DestroyHashMap(HashMap * hashmap);

int HashMapSize(HashMap * hashmap);

void * AddToHashMap(HashMap * hashmap, void * element);
void * GetFromHashMap(HashMap * hashmap, void * element);
void * RemoveFromHashmap(HashMap * hashmap, void * element);
void ClearHashMap(HashMap * hashmap);

typedef struct HashMapIterator HashMapIterator;

HashMapIterator * NewHashMapIterator(HashMap * hashmap);
void DestroyHashMapIterator(HashMapIterator * iterator);

void * GetIteratorCurrent(HashMapIterator * iterator);
void AdvanceIterator(HashMapIterator * iterator);

#endif // HASHMAP_H
