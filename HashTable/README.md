# Hashtable Data Structure
This hashtable structure allows you to store abstract memory references that will enable O(1) addition, lookup and extraction (on a good day).

## How Do I Use It?
`./HashTableTest` has examples of how to make, add to, delete from and query a HashTable. The cool thing about this HashTable is that it acts like a simplified List data structure with better access speeds. This HashTable also contains a useful feature where a user can iterate over all the contents of the HashTable exactly as you'd iterate over a list.

* Creation: `HashTable * hashtable = NewHashTable(CopyFunc cf, HashFunc hf, AreEqualFunc aef, int hashSlots);
;` 
	* where `cf` a pointer to a function that returns a new allocated copy of the passed memory reference
	* where `hf` is a pointer to a hashfunction that takes an abstract reference and returns an integer hash
	* where `aef` is a pointer to a function that returns 1 if two of the abstract elements are equal to one another
	* where `hashSlots` describes the number of buckets in the table. (Sorry, no dynamic resizing yet!)

* Addition: `void * copy = AddToHashTable(HashTable * hashtable, void * element);` adds the element pointed at by creating a copy of the element and inserting it into the table

* Extraction: `void * extracted = ExtractFromHashTable(HashTable * hashtable, void * key);` removes and returns the first element that is equal to the key if present

* Destruction: `void DestroyHashTable(HashTable * hashtable);` cleans up all of its memory allocations

* Snapshot as a List: `List * snapshot = HashTableToList(HashTable * hashtable);` will create a new list of containing all the contents of the HashTable.

* Iteration: iterates over all elements in a HashTable a given point of time (like the snapshot above)
```
HashTable * hashtable; 

// ...
// create and add stuff to hashtable
// ...

HashTableIterator * hti = NewHashTableIterator(hashtable);
void * current = GetHashTableIteratorCurrent(hti);
while (current) {
	
	// do stuff to current

	current = AdvanceAndGetFromHashTableIterator(hti);
}
DestroyHashTableIterator(hti);
```
* And more! Checkout `HashTable.h`

## How Do I Build It?
See `./Makefile` for an example on how to create the `HashTable.o` object file and how to link to it.

```
PATH_TO_HASHTABLE_DIR= # todo -- change this to the path to the HashTable dir
PATH_TO_LIST_DIR= # todo -- change this to the path to the List dir

$(PATH_TO_HASHTABLE_DIR)HashTable.o: $(PATH_TO_HASHTABLE_DIR)HashTable.c $(PATH_TO_HASHTABLE_DIR)HashTable.h
	$(CC) $(CFLAGS) -c $(PATH_TO_HASHTABLE_DIR)HashTable.c -o $@

$(PATH_TO_LIST_DIR)List.o: $(PATH_TO_LIST_DIR)List.c $(PATH_TO_LIST_DIR)List.h
	$(CC) $(CFLAGS) -c $(PATH_TO_LIST_DIR)List.c -o $@
```

# FAQs and Notes

## Claiming Memory -- Who Owns What?
This HashTable creates and maintains its own copy of any data that you pass it in. The `HashTable.h` file explains how returned memory references are owned. If something is extracted, then the caller needs to claim the memory. If something is merely "looked at," then the HashTable will own it. `HashTable.h` will clearly state when memory is claimed or copied. Don't claim memory that you aren't supposed to. Things will segfault.