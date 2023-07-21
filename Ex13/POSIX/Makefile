CC = gcc
CFLAGS = -Wall -Werror -Wextra 
.PHONY : clean

all : main

main : main.o
	$(CC) $(CFLAGS) -o $@ $^ -lrt

main.o : main.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean :
	rm main *.o