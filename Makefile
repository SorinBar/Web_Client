CC=gcc
CFLAGS=-I.

all: build run clean

build: client.c requests.c helpers.c buffer.c parson.c -lm
	$(CC) -o client client.c requests.c helpers.c buffer.c parson.c -lm -Wall

run: 
	./client

test_run:
	valgrind ./client

clean:
	rm -f *.o client
