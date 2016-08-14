#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

typedef struct _Node {
	struct _Node * next, * prev;
	void * data;
} _Node;

typedef struct _List {
	_Node * head, * tail;
	int size;
	void * (*copier)(void *);
} _List;

/* --- internal list functions --- */

_List * _MakeEmptyList() {
	_List * list = (_List *)calloc(1,sizeof(_List));
	assert(list);
	return list;
}

void _ListApply(_List * list, void (*_NodeApplyFunc)(_Node *)) {
	if (!list || !_NodeApplyFunc)
		return;

	_Node * head = list->head;
	_Node * past;
	while (head) {
		past = head;
		head = head->next;
		_NodeApplyFunc(past);
	}
}

/* --- internal node functions --- */

_Node * _MakeEmptyNode() {
	_Node * node = (_Node *)calloc(1,sizeof(_Node));
	assert(node);
	return node;
}

_Node * _MakeNode(void * data, _Node * prev, _Node * next) {
	_Node * node = _MakeEmptyNode();
	node->data = data;
	node->prev = prev;
	node->next = next;
	return node;
}

void _DestroyNode(_Node * node) {
	if (!node)
		return;
	free(node->data);
	free(node);
}

// void _NodeApply(void (*toApplyFunc)(_Node *), _Node * applyTo) {
// 	toApplyFunc(applyTo);
// }

/* --- external --- */

List * NewList(void * (*CopyInFunc)(void *)) {
	_List * new = _MakeEmptyList();
	new->copier = CopyInFunc;
	new->size = 0;
	new->head = new->tail = NULL;
	return (List *)new;
}

void DestroyList(List * list) {
	if (!list)
		return;

	_List * l = (_List *)list;

	_ListApply(l, _DestroyNode);
	free(l);
}

int ListSize(List * list) {
	if (!list)
		return -1;
	_List * l = (_List *)list;
	return l->size;
}

int AppendToList(List * list, void * element) {
	if (!list || !element) {
		return -1;
	}

	_List * l = (_List *)list;
	_Node * tail = l->tail;
	void * copy = l->copier(element);
	if (tail) {
		tail->next = _MakeNode(copy, tail, NULL);
		l->tail = tail->next;
	} else {
		l->head = l->tail = _MakeNode(copy, NULL, NULL);
	}

	return ++l->size;
}