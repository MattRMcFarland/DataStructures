#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct HashTable HashTable;

typedef void * (*CopyFunc)(void * element);
typedef unsigned (*HashFunc)(void * element);
typedef int (*AreEqualFunc)(void * element1, void * element2);
typedef void (*HashTableApplyFunc)(void * element);

HashTable * NewHashTable(CopyFunc cf, HashFunc hf, AreEqualFunc aef, int total);
void DestroyHashTable(HashTable * hashtable);

int HashTableSize(HashTable * hashtable);

// paranoidly returns reference to copied element -- do not claim!
void * AddToHashTable(HashTable * hashtable, void * element);
void * ExtractFromHashTable(HashTable * hashtable, void * key);

// removes all instances of 'key' from the hashtable, returns count of removed elements
int RemoveFromHashTable(HashTable * hashtable, void * key);
void ClearHashTable(HashTable * hashtable);

// returns 1 if present, 0 if not, -1 on error
int HashTableContains(HashTable * hashtable, void * key);

// returns mutated hashtable, does not claim original
HashTable * ApplyToHashTable(HashTable * hashtable, HashTableApplyFunc apply);

HashTable * CopyHashTable(HashTable * hashtable);
void PrintHashTable(HashTable * hashtable, HashTableApplyFunc printer);

/* --- iterate over all elements in a snapshot of hashtable --- */

typedef struct HashTableIterator HashTableIterator;

HashTableIterator * NewHashTableIterator(HashTable * hashtable);
void DestroyHashTableIterator(HashTableIterator * iterator);

void * GetHashTableIteratorCurrent(HashTableIterator * iterator);
void * AdvanceAndGetFromHashTableIterator(HashTableIterator * iterator);

#endif // HASHTABLE_H
