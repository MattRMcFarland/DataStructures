#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../HashMap/HashMap.h"
#include "../AbstractHelpers/StringHelper.h"
#include "Dictionary.h"

Dictionary * NewDictionary(
	ValueCopyFunc vcf, 
	ValueDestroyFunc vdf,
	ValuesAreEqualFunc vaef,
	unsigned int slots) 
{
	if (!vcf || !vdf || !vaef)
		return NULL;
	HashMap * dictionary = NewHashMap(
		&myStrdup, 
		vcf,
		&safeFree,
		vdf,
		&hashPJW,
		&strIsEqual,
		vaef,
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

void PrintDictionary(Dictionary * dictionary, Printer valuePrinter) {
	PrintHashMap((HashMap *)dictionary, &printStr, valuePrinter);
}


