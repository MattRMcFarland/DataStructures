#include "NotImplemented.h"
#include <stdio.h>
#include <assert.h>

void NotImplemented(char * message) {
	fprintf(stderr,"NOT IMPLEMENTED: \"%s\"\n", message);
	assert(0);
}
