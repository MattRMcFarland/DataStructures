# TestingHelper
These are some unit test helper functions.

## How to use
- `shouldBe_Int(int testVal, int targetVal)` compares a `testVal` and a `targetVal`. If the integer values mismatch, an assertion will fail and the test program will terminate.
- `shouldBe_Str(char * testStr, char * targetStr)` compares two strings and exit if the strings are not equal (as indicated by `strcmp` returning 0). Two NULL arguments will be considered equal, but one NULL argument will cause the assertion to fail.
- CustomTester: TODO

## How to make

Add something to the following effect in any dependent Makefiles. You can access the functions in `TestingHelper.h` by including the local library with
 
```
#include ../TestingHelper/TestingHelper.h
```

Any executables that rely on functions in this library must depend on the TestingHelper.o object files. These files can be created with the following Makefile target. (Fix your directory structure to make sure things align!)

```
../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
        $(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@
``` 
