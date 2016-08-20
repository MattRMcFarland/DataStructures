#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMap HashMap;

typedef void * (*CopyFunc)(void * element);
typedef unsigned (*HashFunc)(void * element);
typedef int (*AreEqualFunc)(void * element1, void * element2);
typedef void (*HashMapApplyFunc)(void * element);

HashMap * NewHashMap(CopyFunc cf, HashFunc hf, AreEqualFunc aef, int total);
void DestroyHashMap(HashMap * hashmap);

int HashMapSize(HashMap * hashmap);

// paranoidly returns reference to copied element -- do not claim!
void * AddToHashMap(HashMap * hashmap, void * element);
void * ExtractFromHashMap(HashMap * hashmap, void * key);

// removes all instances of 'key' from the HashMap, returns count of removed elements
int RemoveFromHashmap(HashMap * hashmap, void * key);
void ClearHashMap(HashMap * hashmap);

// returns 1 if present, 0 if not, -1 on error
int HashMapContains(HashMap * hashmap, void * key);

void ApplyToHashMap(HashMap * hashmap, HashMapApplyFunc apply);
HashMap * CopyHashMap(HashMap * hashmap);
void PrintHashMap(HashMap * hashmap, HashMapApplyFunc printer);

typedef struct HashMapIterator HashMapIterator;

HashMapIterator * NewHashMapIterator(HashMap * hashmap);
void DestroyHashMapIterator(HashMapIterator * iterator);

void * GetIteratorCurrent(HashMapIterator * iterator);
void AdvanceIterator(HashMapIterator * iterator);

#endif // HASHMAP_H
