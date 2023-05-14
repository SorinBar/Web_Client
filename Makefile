CC=gcc
CFLAGS=-I.

all: client run clean

client: client.c requests.c helpers.c buffer.c parson.c
	$(CC) -o client client.c requests.c helpers.c buffer.c parson.c -Wall

run: client
	valgrind ./client

clean:
	rm -f *.o client
