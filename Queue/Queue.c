#include <stdlib.h>
#include <assert.h>
#include "../List/List.h"
#include "Queue.h"

typedef struct _Queue {
	List * contents;
} _Queue;

Queue * MakeQueue(CopyFunc copier, DestroyFunc destroyer) {
	if (!copier)
		return NULL;

	_Queue * new = (_Queue *)calloc(1,sizeof(_Queue));
	assert(new);
	new->contents = NewList(copier, destroyer);
	return (Queue *)new;
}

void DestroyQueue(Queue * queue) {
	if (!queue)
		return;

	_Queue * q = (_Queue *)queue;
	DestroyList(q->contents);
	free(q);
	return;
}

void * Enqueue(Queue * queue, void * element) {
	if (!queue || !element)
		return NULL;

	_Queue * q = (_Queue *)queue;
	return AppendToList(q->contents, element);

}
void * Dequeue(Queue * queue) {
	if (!queue)
		return NULL;

	_Queue * q = (_Queue *)queue;
	return TakeHead(q->contents);
}
