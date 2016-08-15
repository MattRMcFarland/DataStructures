# TestingHelper
These are some unit test helper functions.

## How to use

## How to make

Add something to the following effect in any dependent Makefiles. You can access the functions in `TestingHelper.h` by by including the local library with `#include ../TestingHelper/TestingHelper.h`

Any executables that rely on functions in this library must depend on the TestingHelper.o object files. These files can be created with the following Makefile target. (Fix your directory structure to make sure things align!)

```
../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
        $(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@
``` 
