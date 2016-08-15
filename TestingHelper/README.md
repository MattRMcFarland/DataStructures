# TestingHelper
These are some unit test helper functions.

## How to use

## How to make

Add something to the following effect in any dependent Makefiles. You can access the functions in `TestingHelper.h` by by including the local library with `#include `../TestingHelper/TestingHelper.h"

```
../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
        $(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@
``` 
