CC = gcc
CFLAGS = -Wall
.PHONY : clean

all : main

main : main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean :
	rm main *.o