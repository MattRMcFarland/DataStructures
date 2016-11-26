#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../HashMap/HashMap.h"
#include "../AbstractHelpers/StringHelper.h"
#include "Dictionary.h"

Dictionary * NewDictionary(
	CopyFunc valueCopier, 
	DestroyFunc valueDestroyer,
	AreEqualFunc valueComparator,
	unsigned int slots) 
{
	if (!valueCopier || !valueDestroyer || !valueComparator)
		return NULL;
	HashMap * dictionary = NewHashMap(
		&myStrdup, 
		valueCopier,
		&safeFree,
		valueDestroyer,
		&hashPJW,
		&strIsEqual,
		valueComparator,
		slots);
	return (Dictionary *)dictionary;
}

void DestroyDictionary(Dictionary * dictionary) {
	DestroyHashMap((HashMap *)dictionary);
}

int GetDictionarySize(Dictionary * dictionary) {
	return GetHashMapSize((HashMap *)dictionary);
}

void * AddDefinition(Dictionary * dictionary, char * word, void * value) {
	return AddToHashMap((HashMap *)dictionary, word, value);
}

void * Lookup(Dictionary * dictionary, char * word) {
	return GetValueFromHashMap((HashMap *)dictionary, word);
}

int IsDefined(Dictionary * dictionary, char * word) {
	return ContainsKey((HashMap *)dictionary, word);
}

void * RemoveDefinition(Dictionary * dictionary, char * word) {
	return ExtractFromHashMap((HashMap *)dictionary, word);
}

void PrintDictionary(Dictionary * dictionary, ApplyFunc valuePrinter) {
	PrintHashMap((HashMap *)dictionary, &printStr, valuePrinter);
}


