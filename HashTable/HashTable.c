#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../List/List.h"
#include "HashTable.h"

#define DEFAULT_TOTAL 1000;

typedef struct _Bucket {
	List * entries;
} _Bucket;

typedef struct _HashTable {

	CopyFunc copier;
	HashFunc hasher;
	AreEqualFunc judger;

	int size;
	int occupied;
	int total;
	_Bucket ** buckets;
	
} _HashTable;

unsigned DefaultHashFunction(void * element) {
	return (unsigned)(long)element;
}

_Bucket * _HashToBucket(_HashTable * HashTable, void * element) {
	if (!HashTable || !element)
		return NULL;
	return HashTable->buckets[HashTable->hasher(element) % HashTable->total];
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
	return ExtractFromList(bucket->entries, (ListSearchFunc)judger, key);
}

int _RemoveFromBucket(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return 0;
	return RemoveFromList(bucket->entries, (ListSearchFunc)judger, key);
}

void _ClearBucket(_Bucket * bucket) {
	if (!bucket)
		return;
	ClearList(bucket->entries);
}

int _BucketContains(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return -1;
	return ListContains(bucket->entries, (ListSearchFunc)judger, key);
}

void _PrintBucket(_Bucket * bucket, HashTableApplyFunc printer) {
	if (!bucket || !printer)
		return;
	PrintList(bucket->entries, (ListApplyFunc)printer);
}

/* --- internal HashTable --- */

typedef struct _UnsetHashTable _UnsetHashTable;
typedef struct _UnfilledHashTable _UnfilledHashTable;

_UnsetHashTable * _MakeEmptyHashTable() {
	_HashTable * new = (_HashTable *)calloc(1,sizeof(_HashTable));
	assert(new);
	return (_UnsetHashTable *) new;
}

_UnfilledHashTable * _SetEmptyHashTable(
	_UnsetHashTable * unset, 	
	CopyFunc copier, 
	HashFunc hasher, 
	AreEqualFunc judger,
	int total
) {
	if (!unset || !copier || !hasher || !judger || total < 0)
		return NULL;

	_HashTable * HashTable = (_HashTable *)unset;
	HashTable->copier = copier;
	HashTable->hasher = hasher;
	HashTable->judger = judger;
	HashTable->total = total;
	return (_UnfilledHashTable *)HashTable;
}

_HashTable * _FillHashTable(_UnfilledHashTable * unfilled) {
	if (!unfilled)
		return NULL;

	_HashTable * HashTable = (_HashTable *)unfilled;
	HashTable->occupied = 0;
	HashTable->buckets = (_Bucket **)calloc(HashTable->total,sizeof(_Bucket *));
	assert(HashTable->buckets);
	for (int i = 0; i < HashTable->total; i++) {
		HashTable->buckets[i] = _MakeEmptyBucket(HashTable->copier);
	}
	return HashTable;
}

void _ApplyToAllEntries(_HashTable * HashTable, ListApplyFunc apply) {
	if (!HashTable || !apply)
		return;
	for (int i = 0; i < HashTable->total; i++) {
		ListApply(HashTable->buckets[i]->entries, apply);
	}
}

// :( missing partial function support 
void _ApplyToAllLists(_HashTable * HashTable, void (* applyToListFunc)(List *)) {
	if (!HashTable || !applyToListFunc)
		return;
	for (int i = 0; i < HashTable->total; i++) {
		applyToListFunc(HashTable->buckets[i]->entries);
	}
}

void _ApplyToAllBuckets(_HashTable * HashTable, BucketApply apply) {
	if (!HashTable || !apply)
		return;
	for (int i = 0; i < HashTable->total; i++) {
		apply(HashTable->buckets[i]);
	}
}

/* --- external HashTable functions --- */

HashTable * NewHashTable(
	CopyFunc cf, 
	HashFunc hf, 
	AreEqualFunc aef,
	int total
) {
	return (HashTable *)_FillHashTable(_SetEmptyHashTable(_MakeEmptyHashTable(), cf, hf, aef, total));
}

void DestroyHashTable(HashTable * HashTable) {
	if (!HashTable)
		return;

	_HashTable * h = (_HashTable *)HashTable;
	_ApplyToAllBuckets(h, &_DestroyBucket);
	free(h);
}

int HashTableSize(HashTable * HashTable) {
	if (!HashTable)
		return -1;
	_HashTable * h = (_HashTable *)HashTable;
	return h->size;
}

void * AddToHashTable(HashTable * HashTable, void * element) {
	if (!HashTable || !element)
		return NULL;

	_HashTable * h = (_HashTable *)HashTable;
	h->size++;
	return _AddToBucket(_HashToBucket(h, element), element);
}

void * ExtractFromHashTable(HashTable * HashTable, void * key) {
	if (!HashTable)
		return NULL;
	_HashTable * h = (_HashTable *)HashTable;
	void * extracted = _ExtractFromBucket(_HashToBucket(h, key), h->judger, key);
	if (extracted != NULL) {
		h->size--;
	}
	return extracted;
}

int RemoveFromHashTable(HashTable * HashTable, void * key) {
	if (!HashTable)
		return -1;
	_HashTable * h = (_HashTable *)HashTable;
	int removed = _RemoveFromBucket(_HashToBucket(h, key), h->judger, key);
	h->size -= removed;
	return removed;
}

void ClearHashTable(HashTable * HashTable) {
	if (!HashTable)
		return;
	_HashTable * h = (_HashTable *)HashTable;
	h->size = 0;
	_ApplyToAllLists(h, &ClearList);
}

int HashTableContains(HashTable * HashTable, void * key) {
	if (!HashTable)
		return -1;
	_HashTable * h = (_HashTable *)HashTable;
	return _BucketContains(_HashToBucket(h, key), h->judger, key);
}

// nope! need to be more careful than this... need rehash everything into a new map
void ApplyToHashTable(HashTable * HashTable, HashTableApplyFunc apply) {
	

	if (!HashTable || !apply)
		return;
	_HashTable * h = (_HashTable *)HashTable;

	// make a new empty hashmap

	fprintf(stderr,"NOPE! Apply to HashTable isn't supported yet!\n");
	assert(0);
	// go through each element, apply it, then rehash it and add it to the new map
	// don't forget to free the previous memory!

	_ApplyToAllEntries(h, (ListApplyFunc)apply);
}

void PrintHashTable(HashTable * HashTable, HashTableApplyFunc printer) {
	if (!HashTable || !printer)
		return;
	_HashTable * h = (_HashTable *)HashTable;
	printf("HashTable:\n");
	for (int i = 0; i < h->total; i++) {
		if (ListSize(h->buckets[i]->entries) > 0) {
			printf("\tBucket - %d : ", i);
			_PrintBucket(h->buckets[i], printer);
		}
	}
	printf("\n");

}
