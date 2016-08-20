# Linked-List Data Structure
Need a robust list to drop into any of your projects? Try this!

## What Is This?
This is a linked-list structure that holds abstracted memory references. You can create any memory structure and pass it to the list as `(void *)`. The list will create its own copy of the data you've entered and maintain that. When you instantiate a list, you'll need to pass it a `CopyInFunc` function pointer which points to a function that you define that will copy data elements as they are added to the list. I've outlined two examples for Ints and Strings in `./ListTEst`. 

## How Do I Use It?
`./ListTest` has some pretty exhuastive example on how to use the list and all the things you can do with it.

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

```
int myCompareFunc(void * myStruct1, void * myStruct2) {
	// returns 1 if these two data structures are equal, else returns 0
}
```

* Creation: `List * newList = NewList(&copyFunc);`
* Appending: `AppendToList(newList, (void *)&myData);`
* Searching: `ListContains(newList, &myCompareFunc, (void *)&myData);`
* Destruction: `DestroyList(newList);`

## How Do I Build It?
See `./Makefile` for an example of how to make the `List.o` object file and how to link it to executables that rely on it.

```
ListTest: ListTest.c List.o ../TestingHelper/TestingHelper.o
	$(CC) $(CFLAGS) ListTest.c List.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c -o $@
```

## FAQs and Notes
### Claiming Memory -- Who Owns What?
* Caller is responsible for destroying the list after creation.
* When adding items to the list, the caller is responsible for the memory referenced in the argument of the adding function. The list creates a copy of the argument's referenced memory for its internal use. 
* The caller is reponsible for claiming any memory references returned by the list (EXCEPT FOR THE PEEK AND ITERATOR OPERATIONS -- don't claim those!)


### What if I want to pass it a static element with `AppendToList(myIntList, (void *)9000)`?
Sorry, this doesn't work yet. 9000 is loaded into a register for the function call, and it doesn't live in memory (thanks compiler). In order for the copy function to create the list's copy of the data, the original data needs to live in memory.
You can get around this with the following...
```
int dummy = 9000;
AppendToList(myIntList, (void *)&dummy);
```

### Is this threadsafe?
No! ... Or at least not yet...

### How are you checking memory allocations?
If any memory allocations fail, an assertion during the creation function fails, and everything breaks. Sorry if you were hoping for a "nicer" failure behavior, but if you're running out of memory, maybe you have bigger problems.
