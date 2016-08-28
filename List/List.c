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

void * _ExtractData(_Node * node) {
	if (!node)
		return NULL;
	void * extracted = node->data;
	node->data = NULL;
	return extracted;
}

void * _ExtractDataAndDestroy(_Node * node) {
	if (!node)
		return NULL;
	void * data = _ExtractData(node);
	_DestroyNode(node);
	return data;
}

// unsafe! doesn't check list head and tail correctness
_Node * _SpliceOutNode(_Node * node) {
	if (!node)
		return NULL;

	if (node->prev != NULL)
		node->prev->next = node->next;

	if (node->next != NULL)
		node->next->prev = node->prev;

	return node;
}

_Node * _SafeSpliceOutNode(_List * list, _Node * node) {
	if (!list || !node)
		return NULL;

	node = _SpliceOutNode(node);
	list->size--;

	if (node == list->head)
		list->head = node->next;

	if (node == list->tail)
		list->tail = node->prev;

	return node;
}

void _HitchNodes(_Node * caboose, _Node * leader) {
	if (!caboose || !leader)
		return;

	caboose->next = leader;
	leader->prev = caboose;
}

/* --- internal list functions --- */

typedef struct _UnsetList _UnsetList;

_UnsetList * _MakeEmptyList() {
	_List * list = (_List *)calloc(1,sizeof(_List));
	assert(list);
	return (_UnsetList *)list;
}

_List * _SetList(_UnsetList * list, CopyInFunc copier) {
	if (!list)
		return NULL;
	_List * l = (_List *)list;
	l->copier = copier;
	return l; 
}

void * _AppendToList(_List * list, void * element) {
	if (!list || !element)
		return NULL;

	void * copy = list->copier(element);
	assert(copy);
	if (list->tail) {
		list->tail->next = _MakeNode(copy, list->tail, NULL);
		list->tail = list->tail->next;
	} else {
		list->head = list->tail = _MakeNode(copy, NULL, NULL);
	}

	list->size++;
	return copy;
}

void * _PutListHead(_List * list, void * element) {
	if (!list || !element)
		return NULL;

	void * copy = list->copier(element);
	if (list->head) {
		list->head->prev = _MakeNode(copy, NULL, list->head);
		list->head = list->head->prev;
	} else {
		list->head = list->tail = _MakeNode(copy, NULL, NULL);
	}

	list->size++;
	return copy;
}

// sews _Node structs together, free's list2's _List
_List * _SewLists(_List * list1, _List * list2) {
	if (!list1 || !list2)
		return NULL;

	_List * catted;
	if (list1->size > 0) {
		catted = list1;

		if (list2->size > 0) {
			list1->size += list2->size;
			_HitchNodes(list1->tail, list2->head);
			list1->tail = list2->tail;
			free(list2);

		} else {
			free(list2);
		}

	} else {
		catted = list2;
		free(list1);
	}

	return catted;
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

/* --- external functions --- */

List * NewList(CopyInFunc copier) {
	_List * new = _SetList(_MakeEmptyList(), copier);
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

void * AppendToList(List * list, void * element) {
	if (!list || !element)
		return NULL;
	
	_List * l = (_List *)list;
	return _AppendToList(l, element);
}

void * PutListHead(List * list, void * element) {
	if (!list || !element)
		return NULL;

	_List * l = (_List *)list;
	return _PutListHead(l, element);
}

int RemoveFromList(List * list, ListSearchFunc searchFunc, void * key) {
	if (!list || !searchFunc)
		return -1;

	_List * l = (_List *)list;
	_Node * probe = l->head;
	_Node * next;

	int removed = 0;
	while (probe) {
		next = probe->next;
		if (searchFunc(probe->data, key) == 1) {
			_DestroyNode(_SafeSpliceOutNode(l, probe));
			removed++;
		} 
		probe = next;
	}
	return removed;
}

void * ExtractFromList(List * list, ListSearchFunc searchFunc, void * key) {
	if (!list || !searchFunc)
		return NULL;

	_List * l = (_List *)list;
	_Node * probe = l->head;
	while (probe) {
		if (searchFunc(probe->data, key) == 1) {
			return _ExtractDataAndDestroy(_SafeSpliceOutNode(l, probe));;
		}
		probe = probe->next;
	}

	return NULL;
}

int ListContains(List * list, ListSearchFunc searchFunc, void * key) {
	if (!list || !searchFunc)
		return -1;

	_List * l = (_List *)list;
	_Node * probe = l->head;
	while (probe) {
		if (searchFunc(probe->data, key) == 1) {
			return 1;
		}
		probe = probe->next;
	}
	return 0;
}

int ListCount(List * list, ListSearchFunc searchFunc, void * key) {
	if (!list || !searchFunc)
		return -1;

	_List * l = (_List *)list;
	_Node * probe = l->head;
	int count = 0;
	while (probe) {
		if (searchFunc(probe->data, key) == 1) {
			count++;
		}
		probe = probe->next;
	}
	return count;
}


void * TakeHead(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list; 
	return (l->head != NULL) ? _ExtractDataAndDestroy(_SafeSpliceOutNode(l, l->head)) : NULL;
}

void * TakeTail(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list;
	return (l->tail != NULL) ? _ExtractDataAndDestroy(_SafeSpliceOutNode(l, l->tail)) : NULL;
}

void * PeekHead(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list;
	return (l->head != NULL) ? l->head->data : NULL;
}

void * PeekTail(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list;
	return (l->tail != NULL) ? l->tail->data : NULL;
}

List * CatLists(List * list1, List * list2) {
	if (!list1 || !list2)
		return NULL;

	_List * l1 = (_List *)list1;
	_List * l2 = (_List *)list2;

	return (List *)_SewLists(l1, l2);
}

List * CopyList(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list;
	_List * copy = _SetList(_MakeEmptyList(), l->copier);

	_Node * probe = l->head;
	while (probe) {
		_AppendToList(copy,probe->data);
		probe = probe->next;
	}

	return (List *)copy;
}

List * ReverseList(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list;
	_List * reversed = _SetList(_MakeEmptyList(), l->copier);
	_Node * head = l->head;
	while (head) {
		_PutListHead(reversed, head->data);
		head = head->next;
	}
	return (List *)reversed;
}


void ClearList(List * list) {
	if (!list)
		return;

	_List * l = (_List *)list;
	
	_ListApply(l, _DestroyNode);
	l->head = l->tail = NULL;
	l->size = 0;
	return;
}

void ListApply(List * list, ListApplyFunc toApply) {
	if (!list || !toApply)
		return;

	_List * l = (_List *)list;
	_Node * applier = l->head;
	while (applier) {
		toApply(applier->data);
		applier = applier->next;
	}
}

void PrintList(List * list, ListApplyFunc elementPrinter) {
	if (!list || !elementPrinter)
		return;
	_List * l = (_List *)list;
	_Node * pencil = l->head;
	printf("{");
	while (pencil) {
		elementPrinter(pencil->data);
		if (pencil->next)
			printf(", ");

		pencil = pencil->next;
	} 
	printf("}\n");
}


/* --- external data iterator --- */

typedef struct _ListIterator {
	_Node * current;
} _ListIterator;

ListIterator * MakeListIterator(List * list) {
	if (!list)
		return NULL;

	_List * l = (_List *)list;
	_ListIterator * iterator = (_ListIterator *)calloc(1,sizeof(_ListIterator));
	assert(iterator);
	iterator->current = l->head;
	return (ListIterator *)iterator;
}

void DestroyListIterator(ListIterator * iterator) {
	if (!iterator)
		return;
	_ListIterator * i = (_ListIterator *)iterator;
	free(i);
}

void * GetCurrentFromIterator(ListIterator * iterator) {
	if (!iterator)
		return NULL;

	_ListIterator * i = (_ListIterator *)iterator;
	return (i->current != NULL) ? i->current->data : NULL;
}

void * AdvanceAndGetFromIterator(ListIterator * iterator) {
	if (!iterator)
		return NULL;

	_ListIterator * i = (_ListIterator *)iterator;
	if (i->current != NULL) {
		i->current = i->current->next;
	}

	return (i->current != NULL) ? i->current->data : NULL;
}


