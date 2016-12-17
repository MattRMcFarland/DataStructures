#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../List/List.h"
#include "../AbstractHelpers/AbstractFuncs.h"

typedef struct HashTable HashTable;

HashTable * NewHashTable(
	CopyFunc cf, 
	DestroyFunc df, 
	HashFunc hf, 	// use NULL for default hash function 
	AreEqualFunc aef, 
	unsigned int hashSlots);
void DestroyHashTable(HashTable * hashtable);

// returns 1 if both hashtables have same copier, destroyer, hasher, comparer
// returns 0 otherwise
int CompareHashTableStructure(HashTable * hashtable1, HashTable * hashtable2);

int HashTableSize(HashTable * hashtable);

// paranoidly returns reference to copied element -- do not claim!
void * AddToHashTable(HashTable * hashtable, void * element);

void * ExtractFromHashTable(HashTable * hashtable, void * key);

// removes all instances of 'key' from the hashtable, returns count of removed elements
int RemoveFromHashTable(HashTable * hashtable, void * key);

void ClearHashTable(HashTable * hashtable);

// returns 1 if present, 0 if not, -1 on error
int HashTableContains(HashTable * hashtable, void * key);

// returns list of all HashTable contents
List * HashTableToList(HashTable * hashtable);

// returns mutated hashtable, does not claim original
HashTable * ApplyToHashTable(HashTable * hashtable, ApplyFunc apply);

HashTable * CopyHashTable(HashTable * hashtable);

/*
 * -- CopyHashTableStructure --
 * creates a copy of the HashTable (copier, destroyer, comparer, hasher)
 * in an empty hashtable.
 *
 * Note: caller is responsible for returned HashTable memory
 * Usage: HashTable * emptyCopy = CopyHashTableStructure(originalHashTable);
 */
HashTable * CopyHashTableStructure(HashTable * skeleton);

void PrintHashTable(HashTable * hashtable, ApplyFunc printer);

/* --- iterate over all elements in a snapshot of hashtable --- */

typedef struct HashTableIterator HashTableIterator;

HashTableIterator * NewHashTableIterator(HashTable * hashtable);
void DestroyHashTableIterator(HashTableIterator * iterator);

void * GetHashTableIteratorCurrent(HashTableIterator * iterator);
void * AdvanceAndGetFromHashTableIterator(HashTableIterator * iterator);

#endif // HASHTABLE_H
