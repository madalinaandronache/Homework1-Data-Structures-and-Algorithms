CC=gcc
CFLAGS=-Wall -Wextra -std=c99

build: tema1

tema1: tema1.c
	$(CC) $(CFLAGS) -o tema1 tema1.c

run: 
	./tema1

clean:
	rm -f tema1
