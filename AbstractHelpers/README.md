# Abstract Helpers
These functions are helpful to drop into projects that use abstract data structures (like the List or the HashMap) with primitive types like strings and ints.

## AbstractFuncs
Across this project, DataStructures require at least two or three arguments to instantiate a structure. They are function pointers that the user can pass -- these functions are needed to tell the structure how to copy, destroy and compare the data elements that will be stored in the structure.

* `void * (*CopyFunc)(void * element)` points to a function that creates a copy of the argument data element

* `void (*DestroyFunc)(void * element)` points to a function a function that will free the held data types appropriately

* `int (*AreEqualFunc)(void * e1, void * e2)` points to a function that will compare two data elements and return 1 if they are equal (0 otherwise)

* `void (*ApplyFunc)(void * element)` points to a function that does something a data element in place

* `void (*HashMapApplyFunc)(void * key, void * value)` points to a function that modifies a key-value mapped pair of elements (HashMap only)

* `void (*HashFunc)(void * element)` points to a function that hashes off of the data element (HashTable and HashMap)

## IntHelper

* `myIntDup` - the CopyFunc for ints
* `myIntIsEqual` - AreEqualFunc for ints (NULL is least possible)
* `printInt` - ApplyFunc that prints int
* `incInt` - ApplyFunc increments int in place

Note: DestroyFunc for ints should be `safeFree` defined in the StringHelper file

## StringHelper

* `hashPJW` - HashFunc for String hashes
* `myStrdup` - CopyFunc for Strings
* `safeFree` - DestroyFunc for Strings / primitives
* `strIsEqual` - AreEqualFunc for Strings
* `printStr` - ApplyFunc that prints string
* `squasher` - ApplyFunc that turns first byte of String to `_`
* `squashStrStr` - HashMapApplyFunc that squashes key and element (String-String Map)
