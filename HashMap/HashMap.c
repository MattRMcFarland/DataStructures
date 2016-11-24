#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "HashMap.h"
#include "../AbstractHelpers/NotImplemented.h"

/* --- Internal Structure Definition --- */

typedef struct _Entry {
	void * key;
	void * value;
} _Entry;

typedef struct _Bucket {
	List * entries;
} _Bucket;

typedef struct _HashMap {
	KeyCopyFunc keyCopier;
	ValueCopyFunc valueCopier;
	KeyDestroyFunc keyDestroyer;
	ValueDestroyFunc valueDestroyer;
	HashFunc hasher;
	KeysAreEqualFunc keyJudge;
	ValuesAreEqualFunc valueJudge;

	int size;
	int totalBuckets;
	_Bucket ** buckets;

} _HashMap;

unsigned DefaultHashFunction(void * element) {
	return (unsigned)(long)element;
}

_Bucket * _HashToBucket(_HashMap * map, void * key) {
	if (!map || !key)
		return NULL;
	_Bucket * b = map->buckets[map->hasher(key) % map->totalBuckets];
	assert(b); // shouldn't ever get called
	return b;
}

/* --- internal entry --- */

_Entry * _FormEntry(void * copiedKey, void * copiedValue) {
	if (!copiedKey || !copiedValue)
		return NULL;

	_Entry * new = (_Entry *)calloc(1, sizeof(_Entry));
	assert(new);
	new->key = copiedKey;
	new->value = copiedValue;
	return new;
}

void _DestroyEntry(void * e, KeyDestroyFunc keyDestroyer, ValueDestroyFunc valueDestroyer) {
	if (!e || !keyDestroyer || !valueDestroyer)
		return;

	_Entry * entry = (_Entry *)e;
	keyDestroyer(entry->key);
	valueDestroyer(entry->value);
	free(entry);
	return;
}

// prevents LIST implementation from doubly copying
// YOU NEED TO CREATE A COPY OF THE ENTRY AND THEN PASS
// THAT COPY TO THIS FALSE COPY FUNCTION WITH AN AppendToList
void * _FalseAbstractCopy(void * copiedEntry) {
	return copiedEntry;
}

_Entry * _CopyEntry(_Entry * entry, KeyCopyFunc kcf, ValueCopyFunc vcf) {
	if (!entry)
		return NULL;
	_Entry * new = (_Entry *)calloc(1,sizeof(_Entry));
	assert(new);
	new->key = kcf(entry->key);
	new->value = vcf(entry->value);
	return new;
}

void * _UpdateEntryValue(_Entry * entry, ValueDestroyFunc valueDestroyer, void * copiedValue) {
	if (!entry || !valueDestroyer || !copiedValue)
		return NULL;
	valueDestroyer(entry->value);
	entry->value = copiedValue;
	return entry->value;
}

int _ComparePointers(void * e1, void * e2) {
	return (e1 == e2);
}

/* --- internal bucket --- */

_Bucket * _MakeBucket() {
	_Bucket * new = (_Bucket *)calloc(1, sizeof(_Bucket));
	assert(new);
	new->entries = NewList(&_FalseAbstractCopy);
	return new;
}

void _DestroyBucket(_Bucket * bucket) {
	if (!bucket)
		return;
	DestroyList(bucket->entries);
	free(bucket);
	return;
}

void _BucketApply(_Bucket * bucket, HashMapApplyFunc f) {
	if (!bucket || !f)
		return;

	ListIterator * i = MakeListIterator(bucket->entries);
	_Entry * e = GetCurrentFromIterator(i);
	while (e != NULL) {
		f(e->key, e->value);
		e = AdvanceAndGetFromIterator(i);
	}
	DestroyListIterator(i);
}

// returns matching entry if exists
_Entry * _BucketContainsKey(_Bucket * bucket, void * key, KeysAreEqualFunc keyJudge) {
	if (!bucket || !key || !keyJudge)
		return NULL;

	ListIterator * i = MakeListIterator(bucket->entries);
	void * found = NULL;
	_Entry * e = GetCurrentFromIterator(i);
	while (e != NULL && !found) {
		if (keyJudge(e->key, key) == 1) {
			found = e;
		}
		e = AdvanceAndGetFromIterator(i);
	}
	DestroyListIterator(i);
	return found;
}

// returns matching entry if exists
_Entry * _BucketContainsValue(_Bucket * bucket, void * value, ValuesAreEqualFunc valueJudge) {
	if (!bucket || !value || !valueJudge)
		return NULL;

	ListIterator * i = MakeListIterator(bucket->entries);
	void * found = 0;
	_Entry * e = GetCurrentFromIterator(i);
	while (e != NULL && !found) {
		if (valueJudge(e->value, value) == 1) {
			found = e;
		}
		e = AdvanceAndGetFromIterator(i);
	}
	DestroyListIterator(i);
	return found;
}

// returns copied value, do not free
void * _AddToBucket(_Bucket * bucket, void * copiedKey, void * copiedValue) {
	if (!bucket || !copiedKey || !copiedValue)
		return NULL;

	_Entry * toAdd = _FormEntry(copiedKey, copiedValue);	// create the copy here!
	_Entry * added = (_Entry *)AppendToList(bucket->entries, (void *)toAdd);	// will be false copied!
	return added->value;
}

_Entry * _ExtractFromBucket(_Bucket * bucket, void * key, KeysAreEqualFunc keyComparer) {
	if (!bucket || !key || !keyComparer)
		return NULL;

	// note: this approach of manually comparing pointers works because the ListIterator
	// iterates over private contents. If the list iterator returned copies,
	// this function would break hard.

	_Entry * found = _BucketContainsKey(bucket, key, keyComparer);
	return (_Entry *)ExtractFromList(bucket->entries, &_ComparePointers, found);
}

/* --- external Map --- */

HashMap * 
NewHashMap(
	KeyCopyFunc kcf, 
	ValueCopyFunc vcf,
	KeyDestroyFunc kdf,
	ValueDestroyFunc vdf,
	HashFunc hf, 
	KeysAreEqualFunc kaef, 
	ValuesAreEqualFunc vaef,
	unsigned int slots) 
{

	if (!kcf || !vcf || !kdf || !vdf || !kaef || !vaef)
		return NULL;

	_HashMap * map = (_HashMap *)calloc(1, sizeof(_HashMap));
	assert(map);
	map->keyCopier = kcf;
	map->valueCopier = vcf;
	map->keyDestroyer = kdf;
	map->valueDestroyer = vdf;
	map->keyJudge = kaef;
	map->valueJudge = vaef;

	map->hasher = (hf != NULL) ? hf : &DefaultHashFunction;

	map->totalBuckets = slots;
	map->size = 0;

	map->buckets = (_Bucket **)calloc(slots, sizeof(_Bucket *));
	assert(map->buckets);
	for (int i = 0; i < slots; i++) {
		map->buckets[i] = _MakeBucket();
	}

	return (HashMap *)map;
}

void * AddToHashMap(HashMap * m, void * key, void * value) {
	if (!m || !key || !value)
		return NULL;

	_HashMap * map = (_HashMap *)m;
	_Bucket * bucket = _HashToBucket(map, key);
	void * valueCopy = map->valueCopier(value);

	_Entry * entry = _BucketContainsKey(bucket, key, map->keyJudge);
	if (entry != NULL) {
		map->valueDestroyer(entry->value);
		entry->value = valueCopy;
	} else {
		map->size++;
		_AddToBucket(bucket, map->keyCopier(key), valueCopy);
	}

	return valueCopy;
}

void * GetValueFromHashMap(HashMap * m, void * key) {
	if (!m || !key)
		return NULL;

	_HashMap * map = (_HashMap *)m;
	_Bucket * bucket = _HashToBucket(map, key);
	return _BucketContainsKey(bucket, key, map->keyJudge);
}

int ContainsKey(HashMap * map, void * key) {
	return (GetValueFromHashMap(map, key) != NULL) ? 1 : 0;
}

void * ExtractFromHashMap(HashMap * m, void * key) {
	if (!m || !key)
		return NULL;

	_HashMap * map = (_HashMap *)m;
	_Bucket * bucket = _HashToBucket(map, key);

	_Entry * extracted = _ExtractFromBucket(bucket, key, map->keyJudge);
	void * value = NULL;
	if (extracted != NULL) {
		map->size--;
		value = extracted->value;
		extracted->value = NULL;
		_DestroyEntry(extracted, map->keyDestroyer, map->valueDestroyer);
	}
	return value;
}

void HashMapApply(HashMap * m, HashMapApplyFunc f) {
	if (!m || !f)
		return;

	_HashMap * map = (_HashMap *)m;
	for (int i = 0; i < map->totalBuckets; i++) {
		_BucketApply(map->buckets[i], f);
	}
}

void PrintHashMap(HashMap * m, HashMapApplyFunc printer) {
	if (!m || !printer)
		return;

	_HashMap * map = (_HashMap *)m;
	printf("\nMap Print: {Size: %d, Slots: %d}\n", map->size, map->totalBuckets);
	for (int i = 0; i < map->totalBuckets; i++) {
		printf("-- Bucket %d --\n",i);
		printf("\t");
		_BucketApply(map->buckets[i], printer);
		printf("\n");
	}
}

