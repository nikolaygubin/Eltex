explorer : explorer.o
	$(CC) $(CFLAGS) -o $@ $< -lncurses

editor.o : explorer.c
	$(CC) -c $(CFLAGS) explorer.c

run : explorer
	./explorer

clean:
	rm explorer explorer.o