CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

main : main.o
	$(CC) -o main main.o

main.o : main.c
	$(CC) $(CFLAGS) -c main.c


clean :
	rm -f *.o *~ main
