#ifndef STRING_HELPER_H
#define STRING_HELPER_H

unsigned hashPJW(void * string);
void * myStrdup(void * string);
void safeFree(void * string);
int strIsEqual(void * testStr, void * targetStr);
void printStr(void * str);
void squasher(void * string);

#endif // STRING_HELPER_H

