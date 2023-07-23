CC = gcc
CFLAGS = -Wall -O0 -g
.PHONY : clean

all : server client

server : server.o
	$(CC) $(CFLAGS) -o $@ $^

server.o : server.c
	$(CC) -c $(CFLAGS) -o $@ $^

client : client.o
	$(CC) $(CFLAGS) -o $@ $^

client.o : client.c
	$(CC) -c $(CFLAGS) -o $@ $^

run_server : server
	./server

run_client : client
	./client

clean :
	rm client server *.o