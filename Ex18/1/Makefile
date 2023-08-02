.PHONY : clean

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread

all : client server

client : client.o
	$(CC) $(CFLAGS) -o $@ $<
client.o : client.c
	$(CC) -c $(CFLAGS) -o $@ $<

server : server.o
	$(CC) $(CFLAGS) -o $@ $<
server.o : server.c
	$(CC) -c $(CFLAGS) -o $@ $<


clean :
	rm client server *.o
