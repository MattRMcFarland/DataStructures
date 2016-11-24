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
typedef void (*HashTableApplyFunc)(void * key, void * value);

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

void * ExtractFromHashMap(HashMap * map, void * key);

void * ContainsKey(HashMap * map, void * key);
void * GetValueFromHashMap(HashMap * map, void * key);

List * GetKeyList(HashMap * map);

void PrintHashMap(HashMap * map, HashTableApplyFunc printer);

#endif // HASHMAP_H
