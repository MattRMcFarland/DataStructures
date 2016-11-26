# HashMap
This is probably one of the most useful data structures to have around. I can't tell you the number of times I've had to reimplement a hashmap for custom keys and data structures. Hopefully I won't have to do that ever again!

That said, working on this structure has made me really appreciate functional languages. Since I'm using double or triple layers of abstraction (building off of other structures in the project), C's inability to create function instances is pretty painful.

## What Is This?
It's a HashMap that holds keys mapped to values. You provide a way to copy, compare and delete keys / values. You can also provide a hash function (if you don't, I'll just use a default). Both keys and values are `(void *)` which is kinda cool. I'll create copies of keys and values and store them in the structure. Look at function definitions to see when the caller is responsible for claiming memory (really only for value extraction I think).

## How Do I Use It?
`./HashMapTest` has an example of how to use a string-string map. (I recommend using primitives for keys, but you don't have to!)

### CopyFunc
This creates a copy your custom keys or values (supply a destroyer for both keys and values)
```
void * copyFunc(void * data) {
	if (!data)
		return NULL;
	MyStruct * copy = (MyStruct *)calloc(1,sizeof(MyStruct));

	// replicate the information held in data to the newly created copy 

	assert(copy);
	return copy;
}
```

### CompareFunc
This compares your custom keys or values (supply a destroyer for both keys and values)
```
int myCompareFunc(void * myStruct1, void * myStruct2) {
	// returns 1 if these two data structures are equal, else returns 0
}
```

### DestroyerFunc
This deallocates your custom keys or values (supply a destroyer for both keys and values)
```
void myDestroyer(void * myStruct) {
	if (!myStruct)
		return;
	// free all internal struct stuff
	free(myStruct);
}
```

### See `./HashMapTest` for examples on how to execute the follwing operations
* Creation: 
* Addition of Keys / Updating values
* Searching
* iteration

## FAQS and Notes
### Claiming Memory - Who Owns What?
* The HashMap creates internal copies of keys and values. The only time the user owns returned memory segments is specified -- right now that's the `ExtractValueFromHashMap` function

### Threadsafe?
Haha, nice try. Definitely Not.