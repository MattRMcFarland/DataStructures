# TestingHelper
These are some unit test helper functions. By using various `shouldBe` functions, one can ensure that unit tests pass. If a `shouldBe` function call fails, an assertion will halt the program, alerting you to the error. You can then re-run the executable with a debugging program like `lldb` to investigate what went wrong. Building out tests also helped me alter some of these Data Structure projects while ensuring that I wasn't regressing / breaking dependent code.

## How to use
* `shouldBe_Int(int testVal, int targetVal)` compares a `testVal` and a `targetVal`. If the integer values mismatch, an assertion will fail and the test program will terminate.
* `shouldBe_Str(char * testStr, char * targetStr)` compares two strings and exits if the strings are not equal (as indicated by `strcmp` returning 0). Two NULL arguments will be considered equal, but one NULL argument will cause the assertion to fail.
* `shouldBe_NULL(void * testingVal)` tests that the pointer is NULL
* `shouldBe_NonNULL(void * testingVal)` tests that the pointer is non NULL
* CustomTester: You can embed your own `AreEqualFunc` into a custom tester. Then you can pass testing values and expected values into each execution of the tester. I haven't actually used this in any test

## How to make

Add something to the following effect in any dependent Makefiles. You can access the functions in `TestingHelper.h` by including the local library with
 
```
#include "../TestingHelper/TestingHelper.h"
```

Any executables that rely on functions in this library must depend on the TestingHelper.o object files. These files can be created with the following Makefile target. (Fix your directory structure to make sure things align!)

```
../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
        $(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@
``` 
