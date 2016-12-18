# Data Structures
This is a long overdue project. I've been working on creating a solid collection of Data Structures in C that I can drop into other projects of mine. I've built too many one-off HashTables and Linked-Lists to justify procrastinating the abstract implementations.

# Structures

## List
This directory contains a faily robust implementation of an abstract, Linked-List Data Structure. I fold this List into the "under-the-hood" components of many other structures here.

## HashTable
This directory contains an abstract implementation of the HashTable. (Table! not Map.)

## HashMap
This is a fully abstract-key to abstract-value mapping. Includes addition / updating, extraction, and iteration.

## Dictionary
This is an overlay on HashMap. It has string keys by default and abstract values. Pretty easy to use. Same functionality as HashMap without the iterator.

## Queue
Simple overlay on List to implement a Queue.

## Set
Simple overlay on HashTable to implement a Set structure. Has some cool features like `FindIntersection()` and `FindUnion()`. Can convert to a `List` to get all of the extra List functionality.

# Helpers
## Abstract Helpers
Oh, you didn't want to use the List to keep track of custom structs and just want a simple way to track strings or ints? These Abstract Helpers make the abstract data structures easy to use with primitives
* Int Helper
* String Helpers

## TestingHelper
I defined a bunch of testing functions that will cause your test program to `assert(0)` and break hard if an expected value isn't what's observed. These functions help build robust test suites that will help you unit test. I used them for all of these structure tests. Testing assertions include...
* shouldBe_Str(testingStr, expectedStr);
* shouldBe_Int(testingInt, expectedInt);
* shouldBe_IntPtr(testingPtr, expectedPtr);
* shouldBe_NULL(testingVal);
* shouldBe_NonNULL(testingVal);

### Custom Tester
I've included a way to make a custom test (execute a custom comparison function) and assert that two values are equal. I haven't actually used it though.

