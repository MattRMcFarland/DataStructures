CC=gcc
CFLAGS=-Wall -pedantic -g

ListTest: ListTest.c List.o ../TestingHelper/TestingHelper.o ../AbstractHelpers/IntHelper.o ../AbstractHelpers/StringHelper.o
	$(CC) $(CFLAGS) ListTest.c List.o ../TestingHelper/TestingHelper.o ../AbstractHelpers/IntHelper.o ../AbstractHelpers/StringHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

../AbstractHelpers/StringHelper.o: ../AbstractHelpers/StringHelper.c ../AbstractHelpers/StringHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/StringHelper.c -o $@

../AbstractHelpers/IntHelper.o: ../AbstractHelpers/IntHelper.c ../AbstractHelpers/IntHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/IntHelper.c -o $@

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c -o $@

clean:
	rm -rf List.o 
	rm -rf ListTest
	rm -rf ../AbstractHelpers/StringHelper.o
	rm -rf ../AbstractHelpers/IntHelper.o
	rm -rf ../TestingHelper/TestingHelper.o
	rm -rf *.dSYM
