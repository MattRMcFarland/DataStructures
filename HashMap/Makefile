CC=gcc
CFLAGS=-Wall -pedantic -g

HashMapTest: HashMapTest.c HashMap.o ../List/List.o ../AbstractHelpers/StringHelper.o ../TestingHelper/TestingHelper.o ../AbstractHelpers/NotImplemented.o
	$(CC) $(CFLAGS) HashMapTest.c HashMap.o ../List/List.o ../AbstractHelpers/StringHelper.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

../AbstractHelpers/StringHelper.o: ../AbstractHelpers/StringHelper.c ../AbstractHelpers/StringHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/StringHelper.c -o $@

../AbstractHelpers/NotImplemented.o: ../AbstractHelpers/NotImplemented.c ../AbstractHelpers/NotImplemented.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/NotImplemented.c -o $@

HashMap.o: HashMap.c HashMap.h 
	$(CC) $(CFLAGS) -c HashMap.c -o $@

../List/List.o: ../List/List.c ../List/List.h
	$(CC) $(CFLAGS) -c ../List/List.c -o $@

clean:
	rm -rf HashMapTest
	rm -rf HashMap.o
	rm -rf ../List/List.o 
	rm -rf ../TestingHelper/TestingHelper.o
	rm -rf ../AbstractHelpers/StringHelper.o
	rm -rf *.dSYM
