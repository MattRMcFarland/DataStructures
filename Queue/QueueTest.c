#include <stdio.h>
#include "../AbstractHelpers/StringHelper.h"
#include "../TestingHelper/TestingHelper.h"
#include "Queue.h"

int main() {
	printf("\nRunning Queue Tests\n");

	Queue * queue = MakeQueue(&myStrdup, &safeFree);

	shouldBe_Str(Enqueue(queue, NULL), (char *)NULL);

	shouldBe_Str(Enqueue(queue, "one"), "one");
	shouldBe_Str(Enqueue(queue, "two"), "two");
	shouldBe_Str(Enqueue(queue, "three"), "three");
	shouldBe_Str(Enqueue(queue, "four"), "four");
	shouldBe_Str(Enqueue(queue, "five"), "five");

	shouldBe_Str(Dequeue(queue), "one");
	shouldBe_Str(Dequeue(queue), "two");
	shouldBe_Str(Dequeue(queue), "three");
	shouldBe_Str(Dequeue(queue), "four");
	shouldBe_Str(Dequeue(queue), "five");

	shouldBe_Str(Dequeue(queue), (char *)NULL);

	DestroyQueue(queue);

	printf("Queue Tests Pass!\n");
	return 0;
}
