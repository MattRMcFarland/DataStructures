#ifndef ABSTRACT_FUNCS_H
#define ABSTRACT_FUNCS_H

typedef void * (*CopyFunc)(void * element);
typedef void (*DestroyerFunc)(void * element);

typedef int (*AreEqualFunc)(void * element1, void * element2); // returns 1 if equal
typedef void (*ApplyFunc)(void * element);

typedef unsigned (*HashFunc)(void * element);
typedef void (*HashTableApplyFunc)(void * element);
typedef void (*HashMapApplyFunc)(void * key, void * value);


#endif // ABSTRACT_FUNCS_H