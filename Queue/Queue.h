#ifndef QUEUE_H
#define QUEUE_H

typedef void * (*CopyInFunc)(void * element);

typedef struct Queue Queue;

Queue * MakeQueue(CopyInFunc);
void DestroyQueue(Queue * queue);

// paranoidly returns enqueued element -- do not claim!
void * Enqueue(Queue * queue, void * element);

// returns dequeued element -- caller responsible for claiming
void * Dequeue(Queue * queue);

#endif // QUEUE_H

