CC = gcc
CFLAGS = -Wall -Werror -Wextra
.PHONY : clean

all: named_canal unnamed_canal interpretator

named_canal : named_canal.o
	$(CC) $(CFLAGS) -o $@ $<

named_canal.o : named_canal.c
	$(CC) -c $(CFLAGS) -o $@ $<	

unnamed_canal : unnamed_canal.o
	$(CC) $(CFLAGS) -o $@ $<	

unnamed_canal.o : unnamed_canal.c
	$(CC) -c $(CFLAGS) -o $@ $<

interpretator : interpretator.o
	$(CC) $(CFLAGS) -o $@ $<

interpretator.o : interpretator.c
	$(CC) -c $(CFLAGS) -o $@ $<	

run_named : named_canal
	./named_canal

run_unnamed : unnamed_canal
	./unnamed_canal	

run_interpretator : interpretator
	./interpretator		

clean :
	rm named_canal unnamed_canal interpretator *.o