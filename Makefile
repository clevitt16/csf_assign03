CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

csim : main.o csimFuncs.o
	$(CC) -o csim main.o csimFuncs.o

main.o : main.c csimFuncs.h
	$(CC) $(CFLAGS) -c main.c

csimFuncs.o : csimFuncs.c csimFuncs.h
	$(CC) $(CFLAGS) -c csimFuncs.c

clean :
	rm -f *.o *~ main csim
