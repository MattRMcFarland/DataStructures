CC=gcc
CFLAGS=-Wall -pedantic -g

DictionaryTest: DictionaryTest.c Dictionary.o ../HashMap/HashMap.o ../List/List.o ../AbstractHelpers/IntHelper.o ../AbstractHelpers/StringHelper.o ../TestingHelper/TestingHelper.o ../AbstractHelpers/NotImplemented.o
	$(CC) $(CFLAGS) DictionaryTest.c Dictionary.o ../HashMap/HashMap.o ../List/List.o ../AbstractHelpers/IntHelper.o ../AbstractHelpers/StringHelper.o ../TestingHelper/TestingHelper.o -o $@

../TestingHelper/TestingHelper.o: ../TestingHelper/TestingHelper.c ../TestingHelper/TestingHelper.h
	$(CC) $(CFLAGS) -c ../TestingHelper/TestingHelper.c -o $@

../AbstractHelpers/IntHelper.o: ../AbstractHelpers/IntHelper.c ../AbstractHelpers/IntHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/IntHelper.c -o $@

../AbstractHelpers/StringHelper.o: ../AbstractHelpers/StringHelper.c ../AbstractHelpers/StringHelper.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/StringHelper.c -o $@

../AbstractHelpers/NotImplemented.o: ../AbstractHelpers/NotImplemented.c ../AbstractHelpers/NotImplemented.h
	$(CC) $(CFLAGS) -c ../AbstractHelpers/NotImplemented.c -o $@

Dictionary.o: Dictionary.c Dictionary.h
	$(CC) $(CFLAGS) -c Dictionary.c -o $@

../HashMap/HashMap.o: ../HashMap/HashMap.c ../HashMap/HashMap.h 
	$(CC) $(CFLAGS) -c ../HashMap/HashMap.c -o $@

../List/List.o: ../List/List.c ../List/List.h
	$(CC) $(CFLAGS) -c ../List/List.c -o $@

clean:
	rm -rf DictionaryTest
	rm -rf ../HashMap/HashMap.o
	rm -rf ../List/List.o 
	rm -rf ../TestingHelper/TestingHelper.o
	rm -rf ../AbstractHelpers/StringHelper.o
	rm -rf ../AbstractHelpers/IntHelper.o
	rm -rf *.dSYM