#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct HashTable HashTable;

typedef void * (*CopyFunc)(void * element);
typedef unsigned (*HashFunc)(void * element);
typedef int (*AreEqualFunc)(void * element1, void * element2);
typedef void (*HashTableApplyFunc)(void * element);

HashTable * NewHashTable(CopyFunc cf, HashFunc hf, AreEqualFunc aef, int total);
void DestroyHashTable(HashTable * HashTable);

int HashTableSize(HashTable * HashTable);

// paranoidly returns reference to copied element -- do not claim!
void * AddToHashTable(HashTable * HashTable, void * element);
void * ExtractFromHashTable(HashTable * HashTable, void * key);

// removes all instances of 'key' from the HashTable, returns count of removed elements
int RemoveFromHashTable(HashTable * HashTable, void * key);
void ClearHashTable(HashTable * HashTable);

// returns 1 if present, 0 if not, -1 on error
int HashTableContains(HashTable * HashTable, void * key);

void ApplyToHashTable(HashTable * HashTable, HashTableApplyFunc apply);
HashTable * CopyHashTable(HashTable * HashTable);
void PrintHashTable(HashTable * HashTable, HashTableApplyFunc printer);

typedef struct HashTableIterator HashTableIterator;

HashTableIterator * NewHashTableIterator(HashTable * HashTable);
void DestroyHashTableIterator(HashTableIterator * iterator);

void * GetIteratorCurrent(HashTableIterator * iterator);
void AdvanceIterator(HashTableIterator * iterator);

#endif // HASHTABLE_H
