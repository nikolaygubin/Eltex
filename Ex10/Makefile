CC = gcc
CFLAGS = -Wall
.PHONY : clean


all: main

main : main.o
	$(CC) $(CFLAGS) -o $@ $< -pthread

main.o : main.c
	$(CC) -c $(CFLAGS) -o $@ $<	

run :
	./main

clean :
	rm *.o main