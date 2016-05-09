


# myApp.makefile
#
# Guidelines for compiling indexer

CC = gcc
CFLAGS = -Wall -pedantic -std=c11

indexer: indexer.c file.o hashtable.o web.o
	$(CC) $(CFLAGS) -o indexer indexer.c file.o hashtable.o web.o

file.o: file.c file.h
	$(CC) $(CFLAGS) -c file.c

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) -c hashtable.c

web.o: web.c web.h
	$(CC) $(CFLAGS) -c web.c

debug: indexer.c hashtable.c web.c file.c hashtable.h web.h file.h
	$(CC) $(CFLAGS) -g -ggdb -c indexer.c hashtable.c web.c file.c hashtable.h web.h file.h
	$(CC) $(CFLAGS) -g -ggdb -o indexer indexer.o web.o file.o hashtable.o


clean:
	rm indexer file.o hashtable.o web.o
