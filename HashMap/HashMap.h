#ifndef HASHMAP_H
#define HASHMAP_H

#include "../AbstractHelpers/AbstractFuncs.h"
#include "../List/List.h"

typedef struct HashMap HashMap;

/*
 * -- NewHashMap --
 * make a HashMap by providing a bunch of functions for the type
 * of data that you'd like to handle. 
 */
HashMap * NewHashMap(
	CopyFunc keyCopier, 
	CopyFunc valueCopier,
	DestroyerFunc keyDestroyer,
	DestroyerFunc valueDestroyer,
	HashFunc hf, 
	AreEqualFunc keyComparator, 
	AreEqualFunc valueComparator,
	unsigned int slots);

/*
 * -- DestroyHashMap --
 * self-explanatory
 */
void DestroyHashMap(HashMap * map);

/*
 * -- GetHashMapSize --
 * returns the number of mappings, -1 on error
 */
int GetHashMapSize(HashMap * map);

/* 
 * -- AddToHashMap --
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

/*
 * -- ContainsKey --
 * returns 1 if hashmap contains key, 0 otherwise
 */
int ContainsKey(HashMap * map, void * key);

/*
 * -- ExtractFromHashMap --
 * if the passed key exists in the hashmap, the associated
 * value will be removed and returned
 *
 * Note: caller claims returned memory!
 */
void * ExtractFromHashMap(HashMap * map, void * key);

/*
 * -- HashMapApply --
 * caller beware here! This will be applied to each key-value
 * mapping. You can seriously goof up my careful memory management
 * if you aren't careful.
 */
void HashMapApply(HashMap * map, HashMapApplyFunc f);

/*
 * -- PrintHashMap --
 * this is a wrapper about HashMapApply. If you provide a way to print
 * the key and the value, this function will print the map in a 
 * nice JSON'esque way.
 */
void PrintHashMap(HashMap * map, ApplyFunc keyPrinter, ApplyFunc valuePrinter);

/* -- iterate over contents of hashmap -- */

typedef struct HashMapIterator HashMapIterator;

HashMapIterator * NewHashMapIterator(HashMap * hashmap);
void DestroyHashMapIterator(HashMapIterator * iterator);

void GetHashMapIteratorCurrent(
	HashMapIterator * iterator, 
	void ** keyPlaceholder, 
	void ** valuePlaceholder);

void AdvanceAndGetFromHashMapIterator(
	HashMapIterator * iterator, 
	void ** keyPlaceholder, 
	void ** valuePlaceholder);

#endif // HASHMAP_H
