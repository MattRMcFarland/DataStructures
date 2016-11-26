#ifndef STRINGMAP_H
#define STRINGMAP_H

#include "../AbstractHelpers/AbstractFuncs.h"

typedef struct Dictionary Dictionary;

/*
 * -- NewDictionary --
 * Creates a HashMap with string keys and abstract values 
 * ... aka a dictionary!
 */
Dictionary * NewDictionary(
	CopyFunc valueCopier, 
	DestroyFunc valueDestroyer,
	AreEqualFunc valueComparator,
	unsigned int slots);

/*
 * -- DestroyDictionary --
 * self-explanatory
 */
void DestroyDictionary(Dictionary * dictionary);

/*
 * -- GetDictionarySize --
 * returns the number of definitions, -1 on error
 */
int GetDictionarySize(Dictionary * dictionary);

/* 
 * -- AddDefinition --
 * paranoidly returns added definition (NULL on failure)
 * if dictionary already contains key, then the old definition is overwritten
 * and replaced with the new value
 *
 * Note: cannot add NULL keys or NULL values, but you can add the empty string
 * Note: cannot add value if key already exists
 *
 * DO NOT FREE RETURN VALUE!
 */
void * AddDefinition(Dictionary * dictionary, char * word, void * value);

/*
 * -- Lookup --
 * if the passed word exists in the dictionary, the associated
 * definition will be returned. Returns NULL if error occurs or
 * key isn't present.
 * 
 * Note: do not claim return value!
 */
void * Lookup(Dictionary * dictionary, char * word);

/*
 * -- IsDefined --
 * returns 1 if the word is defined, 0 otherwise
 */
int IsDefined(Dictionary * dictionary, char * word);

/*
 * -- RemoveDefinition --
 * if the passed word exists in the dictionary, the associated
 * definition will be removed and returned
 *
 * Note: caller claims returned memory!
 */
void * RemoveDefinition(Dictionary * dictionary, char * word);

/*
 * -- PrintDictionary --
 * prints dictionary nicely provided a way to print the definition values
 * Note: see "../HashMap/HashMap.h" for a definition of a Printer function
 */
void PrintDictionary(Dictionary * dictionary, ApplyFunc valuePrinter);

#endif // STRINGMAP_H
