#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../List/List.h"
#include "HashTable.h"

typedef struct _Bucket {
	List * entries;
} _Bucket;

typedef struct _HashTable {

	CopyFunc copier;
	DestroyFunc destroyer;
	HashFunc hasher;
	AreEqualFunc judger;

	int size;
	int totalBuckets;
	_Bucket ** buckets;
	
} _HashTable;

unsigned DefaultHashFunction(void * element) {
	return (unsigned)(long)element;
}

static _Bucket * _HashToBucket(_HashTable * hashtable, void * element) {
	if (!hashtable || !element)
		return NULL;
	return hashtable->buckets[hashtable->hasher(element) % hashtable->totalBuckets];
}

/* --- internal Bucket --- */

typedef void (*BucketApply)(_Bucket * bucket);

static _Bucket * _MakeEmptyBucket(CopyFunc copier, DestroyFunc destroyer) {
	_Bucket * new = (_Bucket *)calloc(1,sizeof(_Bucket));
	assert(new);
	new->entries = NewList(copier, destroyer);
	return new;
}

static void _DestroyBucket(_Bucket * bucket) {
	if (!bucket)
		return;
	DestroyList(bucket->entries);
	free(bucket);
}

static void * _AddToBucket(_Bucket * bucket, void * element) {
	if (!bucket)
		return NULL;
	return AppendToList(bucket->entries, element);
}

static void * _ExtractFromBucket(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return NULL;
	return ExtractFromList(bucket->entries, judger, key);
}

static int _RemoveFromBucket(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return 0;
	return RemoveFromList(bucket->entries, judger, key);
}

static int _BucketContains(_Bucket * bucket, AreEqualFunc judger, void * key) {
	if (!bucket || !judger)
		return -1;
	return ListContains(bucket->entries, judger, key);
}

static List * _GetBucketList(_Bucket * bucket) {
	return (bucket != NULL) ? bucket->entries : NULL;
}

static _Bucket * _CopyBucket(_Bucket * bucket, CopyFunc copier, DestroyFunc destroyer) {
	if (!bucket)
		return NULL;
	_Bucket * copy = _MakeEmptyBucket(copier, destroyer);
	ListIterator * iterator = MakeListIterator(bucket->entries);
	void * data = GetCurrentFromIterator(iterator);
	while (data) {
		_AddToBucket(copy, data);
		data = AdvanceAndGetFromIterator(iterator);
	}
	DestroyListIterator(iterator);
	return copy;
}

static void _PrintBucket(_Bucket * bucket, ApplyFunc printer) {
	if (!bucket || !printer)
		return;
	PrintList(bucket->entries, printer);
}

/* --- internal HashTable --- */

typedef struct _UnsetHashTable _UnsetHashTable;
typedef struct _UnfilledHashTable _UnfilledHashTable;

static _UnsetHashTable * _MakeEmptyHashTable() {
	_HashTable * new = (_HashTable *)calloc(1, sizeof(_HashTable));
	assert(new);
	return (_UnsetHashTable *) new;
}

static _UnfilledHashTable * _SetEmptyHashTable(
	_UnsetHashTable * unset, 	
	CopyFunc copier,
	DestroyFunc destroyer, 
	HashFunc hasher, 
	AreEqualFunc judger,
	int totalBuckets) {

	if (!unset || !copier || !destroyer || !judger || totalBuckets < 0)
		return NULL;

	_HashTable * hashtable = (_HashTable *)unset;
	hashtable->copier = copier;
	hashtable->destroyer = destroyer;
	hashtable->hasher = (hasher) ? hasher : &DefaultHashFunction;
	hashtable->hasher = hasher;
	hashtable->judger = judger;
	hashtable->totalBuckets = totalBuckets;
	return (_UnfilledHashTable *)hashtable;
}

static _HashTable * _FillHashTable(_UnfilledHashTable * unfilled) {
	if (!unfilled)
		return NULL;

	_HashTable * hashtable = (_HashTable *)unfilled;
	hashtable->buckets = (_Bucket **)calloc(hashtable->totalBuckets,sizeof(_Bucket *));
	assert(hashtable->buckets);
	for (int i = 0; i < hashtable->totalBuckets; i++) {
		hashtable->buckets[i] = _MakeEmptyBucket(hashtable->copier, hashtable->destroyer);
	}
	return hashtable;
}

// :( missing partial function support 
static void _ApplyToAllLists(_HashTable * hashtable, void (* applyToListFunc)(List *)) {
	if (!hashtable || !applyToListFunc)
		return;
	for (int i = 0; i < hashtable->totalBuckets; i++) {
		applyToListFunc(hashtable->buckets[i]->entries);
	}
}

static void _ApplyToAllBuckets(_HashTable * hashtable, BucketApply apply) {
	if (!hashtable || !apply)
		return;
	for (int i = 0; i < hashtable->totalBuckets; i++) {
		apply(hashtable->buckets[i]);
	}
}

static List * _HashTableToList(_HashTable * hashtable) {
	if (!hashtable)
		return NULL;
	List * list = NewList(hashtable->copier, hashtable->destroyer);
	for (int i = 0; i < hashtable->totalBuckets; i++) {
		list = CatLists(list,CopyList(_GetBucketList(hashtable->buckets[i])));
	}
	return list;
}

/* --- internal iterator support --- */

typedef struct _HashTableIterator {
	List * snapshot;
	ListIterator * iterator;
} _HashTableIterator;

// creates a list of all of the hashtable's current entries (in a snapshot copy)!
static _HashTableIterator * _MakeHTI(_HashTable * table) {
	if (!table)
		return NULL;
	_HashTableIterator * hti = (_HashTableIterator *)calloc(1,sizeof(_HashTableIterator));
	assert(hti);
	hti->snapshot = _HashTableToList(table);
	hti->iterator = MakeListIterator(hti->snapshot);
	return hti;
}

static void _DestroyHTI(_HashTableIterator * hti) {
	if (!hti)
		return;
	DestroyListIterator(hti->iterator);
	DestroyList(hti->snapshot);
	free(hti);
}

static void * _GetCurrentFromHTI(_HashTableIterator * hti) {
	if (!hti)
		return NULL;
	return GetCurrentFromIterator(hti->iterator);
}

static void * _AdvanceAndGetFromHTI(_HashTableIterator * hti) {
	if (!hti)
		return NULL;
	return AdvanceAndGetFromIterator(hti->iterator);
}

/* --- external HashTable functions --- */

HashTable * NewHashTable(
	CopyFunc cf, 
	DestroyFunc df,
	HashFunc hf, 
	AreEqualFunc aef,
	unsigned int totalBuckets
) {
	_HashTable * new = 
		_FillHashTable(
			_SetEmptyHashTable(
				_MakeEmptyHashTable(), 
				cf,
				df,
				hf, 
				aef, 
				totalBuckets
			)
		);
	return (HashTable *)new;
}

void DestroyHashTable(HashTable * HashTable) {
	if (!HashTable)
		return;

	_HashTable * h = (_HashTable *)HashTable;
	_ApplyToAllBuckets(h, &_DestroyBucket);
	free(h->buckets);
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

List * HashTableToList(HashTable * hashtable) {
	if (!hashtable)
		return NULL;
	_HashTable * h = (_HashTable *)hashtable;
	return _HashTableToList(h);
}

HashTable * ApplyToHashTable(HashTable * hashtable, ApplyFunc apply) {
	if (!hashtable || !apply)
		return NULL;

	_HashTable * h = (_HashTable *)hashtable;
	_HashTable * new = 		
		_FillHashTable(
			_SetEmptyHashTable(
				_MakeEmptyHashTable(), 
				h->copier, 
				h->destroyer,
				h->hasher, 
				h->judger, 
				h->totalBuckets
			)
		);
 
	List * snapshot = _HashTableToList(h);
	ListApply(snapshot, apply);
	ListIterator * iterator = MakeListIterator(snapshot);
	void * probe = GetCurrentFromIterator(iterator);
	while (probe) {
		_AddToBucket(_HashToBucket(new, probe), probe);
		probe = AdvanceAndGetFromIterator(iterator);
	}

	DestroyListIterator(iterator);
	DestroyList(snapshot);

	return (HashTable *)new;
}

HashTable * CopyHashTable(HashTable * hashtable) {
	if (!hashtable)
		return NULL;
	_HashTable * h = (_HashTable *)hashtable;

	_UnfilledHashTable * unfilledCopy = 
		_SetEmptyHashTable(
			_MakeEmptyHashTable(), 
			h->copier, 
			h->destroyer,
			h->hasher, 
			h->judger, 
			h->totalBuckets
		);

	_HashTable * copy = (_HashTable *)unfilledCopy;
	copy->buckets = (_Bucket **)calloc(copy->totalBuckets,sizeof(_Bucket *));
	assert(copy->buckets);
	for (int i = 0; i < h->totalBuckets; i++) {
		copy->buckets[i] = _CopyBucket(h->buckets[i], h->copier, h->destroyer);
	}
	copy->size = h->size;

	return (HashTable *)copy;
}

void PrintHashTable(HashTable * HashTable, ApplyFunc printer) {
	if (!HashTable || !printer)
		return;
	_HashTable * h = (_HashTable *)HashTable;
	printf("HashTable:\n");
	for (int i = 0; i < h->totalBuckets; i++) {
		if (ListSize(h->buckets[i]->entries) > 0) {
			printf("\tBucket - %d : ", i);
			_PrintBucket(h->buckets[i], printer);
		}
	}
	printf("\n");

}

/* --- external iterator stuff --- */

HashTableIterator * NewHashTableIterator(HashTable * hashtable) {
	if (!hashtable)
		return NULL;
	_HashTable * h = (_HashTable *)hashtable;
	return (HashTableIterator *)_MakeHTI(h);
}

void DestroyHashTableIterator(HashTableIterator * iterator) {
	if (!iterator)
		return;
	_HashTableIterator * hti = (_HashTableIterator *)iterator;
	_DestroyHTI(hti);
	return;
}

void * GetHashTableIteratorCurrent(HashTableIterator * iterator) {
	if (!iterator)
		return NULL;
	_HashTableIterator * hti = (_HashTableIterator *)iterator;
	return _GetCurrentFromHTI(hti);
}

void * AdvanceAndGetFromHashTableIterator(HashTableIterator * iterator) {
	if (!iterator)
		return NULL;
	_HashTableIterator * hti = (_HashTableIterator *)iterator;
	return _AdvanceAndGetFromHTI(hti);
}
