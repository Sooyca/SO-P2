main: main.c
	gcc main.c -pthread -Wall -Werror -o main

run: main
	./main 10 11 3 7
