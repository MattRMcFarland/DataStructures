#ifndef QUEUE_H
#define QUEUE_H

#include "../AbstractHelpers/AbstractFuncs.h"

typedef struct Queue Queue;

Queue * MakeQueue(CopyFunc copier, DestroyFunc destroyer);
void DestroyQueue(Queue * queue);

// paranoidly returns enqueued element -- do not claim!
void * Enqueue(Queue * queue, void * element);

// returns dequeued element -- caller responsible for claiming
void * Dequeue(Queue * queue);

#endif // QUEUE_H

