.PHONY = clean

CC = gcc
CFLAGS = -Wall -Wextra
all : editor

editor : editor.o
	$(CC) $(CFLAGS) -o $@ $< -lncurses

editor.o : editor.c
	$(CC) -c $(CFLAGS) editor.c

run : editor
	./editor test.c

clean:
	rm editor editor.o