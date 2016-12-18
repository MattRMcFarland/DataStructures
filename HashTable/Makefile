CC=gcc
CFLAGS=-Wall -pedantic -g

HashTableTest: HashTableTest.c HashTable.o ../List/List.o ../AbstractHelpers/StringHelper.o ../AbstractHelpers/IntHelper.o ../TestingHelper/TestingHelper.o
	$(CC) $(CFLAGS) HashTableTest.c HashTable.o ../List/List.o ../AbstractHelpers/StringHelper.o ../AbstractHelpers/IntHelper.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

../AbstractHelpers/StringHelper.o: ../AbstractHelpers/StringHelper.c ../AbstractHelpers/StringHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/StringHelper.c -o $@

../AbstractHelpers/IntHelper.o: ../AbstractHelpers/IntHelper.c ../AbstractHelpers/IntHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/IntHelper.c -o $@

HashTable.o: HashTable.c HashTable.h
	$(CC) $(CFLAGS) -c HashTable.c -o $@

../List/List.o: ../List/List.c ../List/List.h
	$(CC) $(CFLAGS) -c ../List/List.c -o $@

clean:
	rm -rf HashTableTest
	rm -rf HashTable.o
	rm -rf ../List/List.o 
	rm -rf ../TestingHelper/TestingHelper.o
	rm -rf ../AbstractHelpers/StringHelper.o
	rm -rf *.dSYM