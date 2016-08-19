#include <stdlib.h>
#include "../List/List.h"
#include "HashMap.h"

#define DEFAULT_TOTAL 1000;

typedef struct _Bucket {
	List * entries;
} _Bucket;

typedef struct _HashMap {

	CopyFunc copier;
	HashFunc hasher;
	AreEqualFunc judger;

	int size;
	int occupied;
	int total;
	_Bucket * buckets[];
	
} _HashMap;

int DefaultHashFunction(void * element) {
	return (int)element
}

_Bucket * _HashToBucket(_HashMap * hashmap, void * element) {
	if (!hashmap || !element)
		return NULL;
	return hashmap->buckets[hashmap->hasher(element) % hashmap->total];
}

/* --- internal Bucket --- */

_Bucket * _MakeEmptyBucket(CopyFunc copier) {
	_Bucket * new = (_Bucket *)calloc(1,sizeof(_Bucket));
	assert(new);
	new->entries = NewList(copier);
	return new;
}

void _DestroyBucket(_Bucket * bucket) {
	if (!bucket)
		return;
	DestroyList(bucket->entries);
	free(bucket);
}

void * _AddToBucket(_Bucket * bucket, void * element) {
	if (!bucket)
		return -1;
	return AppendToList(bucket->entries, element);
}

void * _GetFromBucket(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || judger)
		return NULL;
	return GetFromList(bucket->entries, judger, key);
}



/* --- internal HashMap --- */

typedef struct _UnsetHashMap _UnsetHashMap;
typedef struct _UnfilledHashMap _UnfilledHashMap;

_UnsetHashMap * _MakeEmptyHashMap() {
	_HashMap * new = (_HashMap *)calloc(1,sizeof(_HashMap));
	assert(new);
	return (_UnsetHashMap *) new;
}

_UnfilledHashMap * _SetEmptyHashMap(
	_UnsetHashMap * unset, 	
	CopyFunc copier, 
	HashFunc hasher, 
	AreEqualFunc judger,
	int total
) {
	if (!unset || !copier || !hasher || !judger || total < 0)
		return NULL;

	_HashMap * hashmap = (_HashMap *)unset;
	hashmap->copier = copier;
	hashmap->hasher = hasher;
	hashmap->judger = judger;
	hashmap->total = total;
	return (_UnfilledHashMap *)hashmap;
}

_HashMap * _FillHashMap(_UnfilledHashMap * unfilled) {
	if (!unfilled)
		return NULL;

	_HashMap * hashmap = (_HashMap *)unfilled;
	hashmap->occupied = 0;
	hashmap->buckets = (_Bucket **)calloc(hashmap->total,sizeof(_Bucket *));
	assert(new->buckets);
	for (int i = 0; i < total; i++) {
		hashmap->buckets[i] = _MakeEmptyBucket(copier);
	}
	return hashmap;
}

/* --- external HashMap functions --- */

_HashMap * NewHashMap(
	CopyFunc cf, 
	HashFunc hf, 
	AreEqualFunc aef,
	int total
) {
	return _FillHashMap(
		_SetEmptyHashMap(
			_MakeEmptyHashMap(), cf, hf, aef, total
			)
		);
}

void DestroyHashMap(HashMap * hashmap) {
	if (!hashmap)
		return;

	_HashMap * h = (_HashMap *)hashmap;
	for (int i = 0; i < h->total; i++) {
		_DestroyBucket(h->buckets[i]);
	}
	free(h);
}

int HashMapSize(HashMap * hashmap) {
	if (!hashmap)
		return -1;
	_HashMap * h = (_HashMap *)hashmap;
	return h->size;
}

void * AddToHashMap(HashMap * hashmap, void * element) {
	if (!hashmap || !element)
		return NULL;

	_HashMap * h = (_HashMap *)hashmap;
	return _AddToBucket(_HashToBucket(h, element));
}