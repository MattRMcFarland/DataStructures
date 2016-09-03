CC=gcc
CFLAGS=-Wall -pedantic -g

QueueTest: QueueTest.c Queue.o ../List/List.o ../AbstractHelpers/StringHelper.o ../TestingHelper/TestingHelper.o
	$(CC) $(CFLAGS) QueueTest.c Queue.o ../List/List.o ../AbstractHelpers/StringHelper.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

../AbstractHelpers/StringHelper.o: ../AbstractHelpers/StringHelper.c ../AbstractHelpers/StringHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/StringHelper.c -o $@

Queue.o: Queue.c Queue.h
	$(CC) $(CFLAGS) -c Queue.c -o $@

../List/List.o: ../List/List.c ../List/List.h
	$(CC) $(CFLAGS) -c ../List/List.c -o $@

clean:
	rm -rf QueueTest
	rm -rf Queue.o
	rm -rf ../List/List.o 
	rm -rf ../TestingHelper/TestingHelper.o
	rm -rf ../AbstractHelpers/StringHelper.o
	rm -rf *.dSYM