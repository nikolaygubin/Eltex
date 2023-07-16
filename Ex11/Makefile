.PHONY: clean

all : ncurses_shop shop 

ncurses_shop : ncurses_shop.o
	gcc -o $@ $< -lcurses -pthread

ncurses_shop.o : ncurses_shop.c
	gcc -c -o $@ $< 

shop : shop.o
	gcc -o $@ $< -pthread

shop.o : shop.c
	gcc -c -o $@ $< -pthread

run : shop
	./shop

run_ncurses_shop : ncurses_shop
	./ncurses_shop

clean:
	rm *.o shop ncurses_shop