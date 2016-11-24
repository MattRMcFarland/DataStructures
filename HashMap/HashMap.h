#ifndef HASHMAP_H
#define HASHMAP_H

#include "../List/List.h"

typedef struct HashMap HashMap;

typedef void * (*KeyCopyFunc)(void * key);
typedef void * (*ValueCopyFunc)(void * value);

typedef void (*KeyDestroyFunc)(void * key);
typedef void (*ValueDestroyFunc)(void * value);

typedef int (*KeysAreEqualFunc)(void * key1, void * key2);
typedef int (*ValuesAreEqualFunc)(void * value1, void * value2);

typedef unsigned (*HashFunc)(void * element);
typedef void (*HashMapApplyFunc)(void * key, void * value);

HashMap * 
NewHashMap(
	KeyCopyFunc kcf, 
	ValueCopyFunc vcf,
	KeyDestroyFunc kdf,
	ValueDestroyFunc vdf,
	HashFunc hf, 
	KeysAreEqualFunc kaef, 
	ValuesAreEqualFunc vaef,
	unsigned int slots);

void DestroyHashMap(HashMap * map);

/* 
 * -- AddToHashmap --
 * paranoidly returns added value (NULL on failure)
 * if hashmap already contains key, then the old value is destroyed
 * and replaced with the new value
 *
 * Note: cannot add NULL keys or NULL values
 * Note: cannot add value if key already exists
 *
 * DO NOT FREE RETURN VALUE!
 */
void * AddToHashMap(HashMap * map, void * key, void * value);

/*
 * -- GetValueFromHashMap --
 * if the passed key exists in the hashmap, the associated
 * value will be returned. Returns NULL if error occurs or
 * key isn't present.
 * 
 * Note: do not claim return value!
 */
void * GetValueFromHashMap(HashMap * map, void * key);

// returns 1 if hashmap contains key, 0 otherwise
int ContainsKey(HashMap * map, void * key);

/*
 * -- ExtractFromHashMap --
 * if the passed key exists in the hashmap, the associated
 * value will be removed and returned
 *
 * Note: caller claims returned memory!
 */
void * ExtractFromHashMap(HashMap * map, void * key);



// creates list of keys
// List * GetKeyList(HashMap * map);
// List * GetValueList(HashMap * map);

void HashMapApply(HashMap * map, HashMapApplyFunc f);
void PrintHashMap(HashMap * map, HashMapApplyFunc printer);

/* -- iterate over contents of hashmap -- */

typedef struct HashMapIterator HashMapIterator;

HashMapIterator * NewHashMapIterator(HashMap * hashmap);
void DestroyHashMapIterator(HashMapIterator * iterator);

void GetHashMapIteratorCurrent(
	HashMapIterator * iterator, 
	void ** keyPlaceholder, 
	void ** valuePlaceholder);

void * AdvanceAndGetFromHashMapIterator(
	HashMapIterator * iterator, 
	void ** keyPlaceholder, 
	void ** valuePlaceholder);

#endif // HASHMAP_H
