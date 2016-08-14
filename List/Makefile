CC=gcc
CFLAGS=-Wall -pedantic -g

ListTest: ListTest.c List.o ../TestingHelper/TestingHelper.o
	$(CC) $(CFLAGS) ListTest.c List.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c -o $@

clean:
	rm -rf List.o ListTest ../TestingHelper/TestingHelper.o
	rm -rf *.dSYM
