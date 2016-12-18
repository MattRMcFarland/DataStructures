# Set Data Structure
This Set structure is just an overlay of HashTable. It has some default parameters baked into that HashTable (like number of buckets & hash function). 

## How do I use it?
`./SetTest` has lots of examples on how to make a String set. You can do simple operations like add and remove elements. You can also do some cool operations like find the union and intersection of different sets. Additionally, you can convert the Set to a List, which has a lot more functionality (do this for iteration over the Set!).

* Creation (String Set): `Set * set = NewSet(&myStrdup, &safeFree, &strIsEqual)`
* Add Element: `AddToSet(set, "example Addition");`
* Remove Element: `char * removed = RemoveFromSet(set, "look for this");`
* Check for Presence: `SetContains(set, "contains this?");`
* Find Intersection: `Set * intersection = FindIntersection(set1, set2);`
* Find Union: `Set * unionSet = FindUnion(set1, set2);`
* Iteration: Use `List * list = SetToList(set);` and then `ListIterator * li = MakeListIterator(list)`;

## How do I build it?
See `./Makefile` for the relevant object file recipes.

# FAQS and Notes

## Claiming Memory -- Who Owns What?
The Set creates a copy of passed elements and maintains that internally. These memory references are extracted and passed back to the caller upon extraction. The caller is responsible for destroying that memory. Otherwise this is a pretty self-sufficient structure.

Note: you'll need to claim returned sets for intersection and union operations.