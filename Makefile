CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

all: build run clean

build: client.c requests.c helpers.c buffer.c parson.c -lm
	$(CC) $(CFLAGS) client.c requests.c helpers.c buffer.c parson.c -o client $(LDFLAGS)

run: 
	./client

test_run:
	valgrind ./client

clean:
	rm -f *.o client
