.PHONY : clean

CC = gcc
CFLAGS = -O0 -g -Wall -Werror -Wextra -pthread

all : client server snifer

client : client.o
	$(CC) $(CFLAGS) -o $@ $<
client.o : client.c
	$(CC) -c $(CFLAGS) -o $@ $<

server : server.o
	$(CC) $(CFLAGS) -o $@ $<
server.o : server.c
	$(CC) -c $(CFLAGS) -o $@ $<

snifer : snifer.o
	$(CC) $(CFLAGS) -o $@ $<
snifer.o : snifer.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm snifer client server *.o