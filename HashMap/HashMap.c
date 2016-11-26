#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "HashMap.h"

/* --- Internal Structure Definition --- */

typedef struct _Entry {
	void * key;
	void * value;
} _Entry;

typedef struct _Bucket {
	List * entries;
} _Bucket;

typedef struct _HashMap {
	CopyFunc keyCopier;
	CopyFunc valueCopier;
	DestroyFunc keyDestroyer;
	DestroyFunc valueDestroyer;
	HashFunc hasher;
	AreEqualFunc keyJudge;
	AreEqualFunc valueJudge;

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

void _DestroyEntry(_Entry * entry, DestroyFunc keyDestroyer, DestroyFunc valueDestroyer) {
	if (!entry || !keyDestroyer || !valueDestroyer)
		return;
	keyDestroyer(entry->key);
	valueDestroyer(entry->value);
	free(entry);
	return;
}

void _DestroyEntryList(List * list, DestroyFunc keyDestroyer, DestroyFunc valueDestroyer) {
	if (!list || !keyDestroyer || !valueDestroyer)
		return;

	_Entry * entry = NULL;
	while ( (entry = (_Entry *)TakeHead(list)) != NULL) {
		_DestroyEntry(entry, keyDestroyer, valueDestroyer);
	}
	DestroyList(list);
}

// prevents LIST implementation from doubly copying
// YOU NEED TO CREATE A COPY OF THE ENTRY AND THEN PASS
// THAT COPY TO THIS FALSE COPY FUNCTION WITH AN AppendToList
void * _FalseAbstractCopy(void * copiedEntry) {
	return copiedEntry;
}

void _BreakingDestroy(void * data) {
	fprintf(stderr,"ListDestroy is attempting to delete an entry -- This should never be called!\n");
	assert(0);
}

_Entry * _CopyEntry(_Entry * entry, CopyFunc kcf, CopyFunc vcf) {
	if (!entry)
		return NULL;
	_Entry * new = (_Entry *)calloc(1,sizeof(_Entry));
	assert(new);
	new->key = kcf(entry->key);
	new->value = vcf(entry->value);
	return new;
}

void * _UpdateEntryValue(_Entry * entry, DestroyFunc valueDestroyer, void * copiedValue) {
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

	// I'm using these lists in a super hacky way! Gross!
	new->entries = NewList(&_FalseAbstractCopy, &_BreakingDestroy);
	return new;
}

void _DestroyBucket(_Bucket * bucket, DestroyFunc keyDestroyer, DestroyFunc valueDestroyer) {
	if (!bucket)
		return;
	_DestroyEntryList(bucket->entries, keyDestroyer, valueDestroyer);
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

void _BucketPrint(_Bucket * bucket, ApplyFunc keyPrinter, ApplyFunc valuePrinter) {
	if (!bucket || !keyPrinter || !valuePrinter)
		return;

	ListIterator * i = MakeListIterator(bucket->entries);
	_Entry * e = GetCurrentFromIterator(i);
	while (e != NULL) {
		printf("{");
		keyPrinter(e->key);
		printf(" -> ");
		valuePrinter(e->value);
		printf("} ");		
		e = AdvanceAndGetFromIterator(i);
	}
	DestroyListIterator(i);
}

// returns matching entry if exists
_Entry * _BucketContainsKey(_Bucket * bucket, void * key, AreEqualFunc keyJudge) {
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

// returns copied value, do not free
void * _AddToBucket(_Bucket * bucket, void * copiedKey, void * copiedValue) {
	if (!bucket || !copiedKey || !copiedValue)
		return NULL;

	_Entry * toAdd = _FormEntry(copiedKey, copiedValue);	// create the copy here!
	_Entry * added = (_Entry *)AppendToList(bucket->entries, (void *)toAdd);	// will be false copied!
	return added->value;
}

_Entry * _ExtractFromBucket(_Bucket * bucket, void * key, AreEqualFunc keyComparer) {
	if (!bucket || !key || !keyComparer)
		return NULL;

	// note: this approach of manually comparing pointers works because the ListIterator
	// iterates over private contents. If the list iterator returned copies,
	// this function would break hard.

	_Entry * found = _BucketContainsKey(bucket, key, keyComparer);
	return (_Entry *)ExtractFromList(bucket->entries, &_ComparePointers, found);
}

// returns a copy list of _Entries in bucket
List * _CopyBucketEntries(_Bucket * bucket, CopyFunc keyCopier, CopyFunc valueCopier) {
	if (!bucket || !keyCopier || !valueCopier) 
		return NULL;

	List * copies = NewList(&_FalseAbstractCopy, &_BreakingDestroy);

	ListIterator * i = MakeListIterator(bucket->entries);
	_Entry * e = GetCurrentFromIterator(i);
	while (e != NULL) {
		AppendToList(copies, _FormEntry(keyCopier(e->key), valueCopier(e->value)));
		e = AdvanceAndGetFromIterator(i);
	}
	DestroyListIterator(i);

	return copies;
}

/* --- external Map --- */

HashMap * NewHashMap(
	CopyFunc kcf, 
	CopyFunc vcf,
	DestroyFunc kdf,
	DestroyFunc vdf,
	HashFunc hf, 
	AreEqualFunc kaef, 
	AreEqualFunc vaef,
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

void DestroyHashMap(HashMap * m) {
	if (!m)
		return;

	_HashMap * map = (_HashMap *)m;
	for (int i = 0; i < map->totalBuckets; i++) {
		_DestroyBucket(map->buckets[i], map->keyDestroyer, map->valueDestroyer);
	}
	free(map->buckets);
	free(map);
}


int GetHashMapSize(HashMap * m) {
	if (!m)
		return -1;
	_HashMap * map = (_HashMap *)m;
	return map->size;
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
	_Entry * entry = _BucketContainsKey(bucket, key, map->keyJudge);
	return (entry != NULL) ? entry->value : NULL;
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

void PrintHashMap(HashMap * m, ApplyFunc keyPrinter, ApplyFunc valuePrinter) {
	if (!m || !keyPrinter || !valuePrinter)
		return;

	_HashMap * map = (_HashMap *)m;
	printf("\nMap Print: {Size: %d, Slots: %d}\n", map->size, map->totalBuckets);
	for (int i = 0; i < map->totalBuckets; i++) {
		printf("\tBucket %d : ",i);
		_BucketPrint(map->buckets[i], keyPrinter, valuePrinter);
		printf("\n");
	}
}

/* -- external iterator -- */

typedef struct _HashMapIterator {
	List * entries;
	ListIterator * iterator;

	CopyFunc keyCopier;
	CopyFunc valueCopier;
	DestroyFunc keyDestroyer;
	DestroyFunc valueDestroyer;
} _HashMapIterator;

HashMapIterator * NewHashMapIterator(HashMap * m) {
	if (!m)
		return NULL;

	_HashMap * map = (_HashMap *)m;
	_HashMapIterator * hmi = (_HashMapIterator *)calloc(1, sizeof(_HashMapIterator));
 	assert(hmi);

 	hmi->keyCopier = map->keyCopier;
 	hmi->valueCopier = map->valueCopier;
 	hmi->keyDestroyer = map->keyDestroyer;
 	hmi->valueDestroyer = map->valueDestroyer;

 	hmi->entries = NewList(&_FalseAbstractCopy, &_BreakingDestroy);
 	for (int i = 0; i < map->totalBuckets; i++) {
 		List * copies = _CopyBucketEntries(map->buckets[i], map->keyCopier, map->valueCopier);
 		hmi->entries = CatLists(hmi->entries, copies); 
 	}

 	hmi->iterator = MakeListIterator(hmi->entries);
 	return (HashMapIterator *)hmi;
}

void DestroyHashMapIterator(HashMapIterator * i) {
	if (!i)
		return;

	_HashMapIterator * hmi = (_HashMapIterator *)i;
	DestroyListIterator(hmi->iterator);
	_DestroyEntryList(hmi->entries, hmi->keyDestroyer, hmi->valueDestroyer);
	free(hmi);
	return;
}

void GetHashMapIteratorCurrent(
	HashMapIterator * i, 
	void ** keyPlaceholder, 
	void ** valuePlaceholder) 
{
	if (!i || !keyPlaceholder || !valuePlaceholder)
		return;

	_HashMapIterator * hmi = (_HashMapIterator *)i;
	_Entry * current = (_Entry *)GetCurrentFromIterator(hmi->iterator);
	if (current) {
		*keyPlaceholder = current->key;
		*valuePlaceholder = current->value;
	} else {
		*keyPlaceholder = *valuePlaceholder = NULL;
	}

}

void AdvanceAndGetFromHashMapIterator(
	HashMapIterator * i, 
	void ** keyPlaceholder, 
	void ** valuePlaceholder) 
{
	if (!i || !keyPlaceholder || !valuePlaceholder)
		return;

	_HashMapIterator * hmi = (_HashMapIterator *)i;
	_Entry * current = (_Entry *)AdvanceAndGetFromIterator(hmi->iterator);
	if (current) {
		*keyPlaceholder = current->key;
		*valuePlaceholder = current->value;
	} else {
		*keyPlaceholder = *valuePlaceholder = NULL;
	}
}
