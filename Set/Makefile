CC=gcc
CFLAGS=-Wall -pedantic -g

SetTest: SetTest.c Set.o ../HashTable/HashTable.o ../List/List.o ../AbstractHelpers/StringHelper.o ../AbstractHelpers/IntHelper.o ../TestingHelper/TestingHelper.o
	$(CC) $(CFLAGS) SetTest.c Set.o ../HashTable/HashTable.o ../List/List.o ../AbstractHelpers/StringHelper.o ../AbstractHelpers/IntHelper.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

../AbstractHelpers/StringHelper.o: ../AbstractHelpers/StringHelper.c ../AbstractHelpers/StringHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/StringHelper.c -o $@

../AbstractHelpers/IntHelper.o: ../AbstractHelpers/IntHelper.c ../AbstractHelpers/IntHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/IntHelper.c -o $@

Set.o: Set.c Set.h
	$(CC) $(CFLAGS) -c Set.c -o $@

../HashTable/HashTable.o: ../HashTable/HashTable.c ../HashTable/HashTable.h
	$(CC) $(CFLAGS) -c ../HashTable/HashTable.c -o $@

../List/List.o: ../List/List.c ../List/List.h
	$(CC) $(CFLAGS) -c ../List/List.c -o $@

clean:
	rm -rf SetTest
	rm -rf Set.o
	rm -rf ../HashTable/HashTable.o
	rm -rf ../List/List.o 
	rm -rf ../TestingHelper/TestingHelper.o
	rm -rf ../AbstractHelpers/StringHelper.o
	rm -rf *.dSYM