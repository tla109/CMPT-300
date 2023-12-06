
mem_test: my_malloc.o mem_test.o
	gcc -Wall -o mem_test my_malloc.o mem_test.o && ./mem_test

my_malloc.o: my_malloc.c my_malloc.h
	gcc -Wall -c my_malloc.c

mem_test.o: mem_test.c my_malloc.h
	gcc -Wall -c mem_test.c

clean:
	rm -f mem_test my_malloc.o mem_test.o
