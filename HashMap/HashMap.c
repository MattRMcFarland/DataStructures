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

typedef void (*BucketApply)(_Bucket * bucket);

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
		return NULL;
	return AppendToList(bucket->entries, element);
}

void * _ExtractFromBucket(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return NULL;
	return ExtractFromList(bucket->entries, judger, key);
}

void * _RemoveFromBucket(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return 0;
	return RemoveFromList(bucket->entries, judger, key);
}

void _ClearBucket(_Bucket * bucket) {
	if (!bucket)
		return;
	ClearList(bucket->entries);
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


void _ApplyToAllBuckets(_HashMap * hashmap, BucketApply apply) {
	if (!hashmap || !apply)
		return;
	for (int i = 0; i < hashmap->total; i++) {
		apply(hashmap->buckets[i]);
	}
}

// :( missing partial function support 
void _ApplyToAllLists(_HashMap * hashmap, ListApplyFunc apply) {
	if (!hashmap || !apply)
		return;
	for (int i = 0; i < hashmap->total; i++) {
		apply(hashmap->buckets[i]->entries)
	}
}

/* --- external HashMap functions --- */

_HashMap * NewHashMap(
	CopyFunc cf, 
	HashFunc hf, 
	AreEqualFunc aef,
	int total
) {
	return _FillHashMap(_SetEmptyHashMap(_MakeEmptyHashMap(), cf, hf, aef, total));
}

void DestroyHashMap(HashMap * hashmap) {
	if (!hashmap)
		return;

	_HashMap * h = (_HashMap *)hashmap;
	_ApplyToAllBuckets(h, &_DestroyBucket);
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
	h->size++;
	return _AddToBucket(_HashToBucket(h, element));
}

void * ExtractFromHashMap(HashMap * hashmap, void * key) {
	if (!hashmap)
		return NULL;
	_HashMap * h = (_HashMap *)hashmap;
	void * extracted = _ExtractFromBucket(_HashToBucket(h, key), h->judger, key);
	if (extracted != NULL) {
		h->size--;
	}
	return extracted;
}

int RemoveFromHashMap(HashMap * hashmap, void * key) {
	if (!hashmap)
		return NULL;
	_HashMap * h = (_HashMap *)hashmap;
	int removed = _RemoveFromBucket(_HashToBucket(h, key), h->judger, key);
	h->size -= removed;
	return removed;
}

void ClearHashMap(HashMap * hashmap) {
	if (!hashmap)
		return;
	_HashMap * h = (_HashMap *)hashmap;
	_ApplyToAllLists(h, &ClearList);
}

void ApplyToHashMap(HashMap * hashmap, HashMapApplyFunc apply) {
	if (!hashmap || !apply)
		return NULL;
	_HashMap * h = (_HashMap *)hashmap;
	_ApplyToAllLists(h, (ListApplyFunc)apply);
}