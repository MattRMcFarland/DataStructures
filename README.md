# Data Structures
This is a long overdue project. I've been working on creating a solid collection of Data Structures in C that I can drop into other projects of mine. I've built too many one-off HashTables and Linked-Lists to justify procrastinating the abstract implementations.

# Structures

## List
This directory contains a faily robust implementation of an abstract, Linked-List Data Structure. I fold this List into the "under-the-hood" components of many other structures here.

## HashTable
This directory contains an abstract implementation of the HashTable. (Table! not Map.)

## HashMap
This is a fully abstract-key to abstract-value mapping. Still a work in progress! (This makes me miss Scala a lot :()

## Queue
Simple overlay on List to implement a Queue.

# Helpers
## Abstract Helpers
Oh, you didn't want to use the List to keep track of custom structs and just want a simple way to track strings or ints? These Abstract Helpers make the abstract data structures easy to use with primitives
### Int Helper
### String Helpers

## TestingHelper
I defined a bunch of testing functions that will cause your test program to `assert(0)` and break hard if an expected value isn't what's observed. These functions help build robust test suites that will help you unit test. I used them for all of these structure tests.

