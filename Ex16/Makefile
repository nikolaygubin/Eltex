CC = gcc
CFLAGS = -Wall -Werror -Wextra
.PHONY : clean

all : receiver sender event_loop

receiver : receiver.o
	$(CC) $(CFLAGS) -o $@ $<

receiver.o : receiver.c
	$(CC) -c $(CFLAGS) -o $@ $<	

sender : sender.o
	$(CC) $(CFLAGS) -o $@ $<

sender.o : sender.c
	$(CC) -c $(CFLAGS) -o $@ $<		

event_loop : event_loop.o
	$(CC) $(CFLAGS) -o $@ $<

event_loop.o : event_loop.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean :
	rm receiver sender event_loop *.o