CC = gcc
CFLAGS = -Wall -Werror -Wextra
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


clean :
	rm client server *.o

