.PHONY = clean

build_static : div.c mult.c process.c sub.c sum.c
	gcc -c $^
	ar rc libmath.a *.o

static_compile : build_static
	gcc -Wall main.c -o main -L. -lmath

build_dynamic : sum.c sub.c mult.c div.c process.c
	gcc -fPIC -c $^
	gcc -shared -o libmath.so *.o

dynamic_compile : build_dynamic
	gcc main.c -L. -lmath -o main

plugin_compile : build_dynamic
	gcc -Wall -o dynamic_main dynamic_main.c -ldl

run_static : static_compile
	./main

run_dynamic : dynamic_compile
	LD_LIBRARY_PATH=$$PWD ./main

run_plugins : plugin_compile
	./dynamic_main

clean:
	rm *.o *.a *.so main dynamic_main