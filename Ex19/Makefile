.PHONY : clean

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread

all : snifer

snifer : snifer.o
	$(CC) $(CFLAGS) -o $@ $<
snifer.o : snifer.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm snifer *.o