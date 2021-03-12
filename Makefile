CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

csim : main.o csimFuncs.o
	$(CC) -o csim main.o csimFuncs.o -lm

csimTests : csimFuncsTests.o csimFuncs.o tctest.o
	$(CC) -o csimTests csimFuncsTests.o csimFuncs.o tctest.o -lm

main.o : main.c csimFuncs.h
	$(CC) $(CFLAGS) -c main.c

csimFuncs.o : csimFuncs.c csimFuncs.h
	$(CC) $(CFLAGS) -c csimFuncs.c

csimFuncsTests.o : csimFuncsTests.c csimFuncs.h
	$(CC) $(CFLAGS) -c csimFuncsTests.c

tctest.o : tctest.c tctest.h
	$(CC) $(CFLAGS) -c tctest.c



clean :
	rm -f *.o *~ main csim
