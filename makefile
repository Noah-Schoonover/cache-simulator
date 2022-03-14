CC=gcc
CFLAGS=-g -Wall
CFLAGS_OBJ=-g -Wall -c

all: lab2

#============================================================ lab2

config.o: config.h config.c
	$(CC) $(CFLAGS_OBJ) config.c

trace.o: trace.h trace.c
	$(CC) $(CFLAGS_OBJ) trace.c

lab2: lab2.c config.o trace.o
	$(CC) $(CFLAGS_OBJ) lab2.c
	$(CC) $(CFLAGS) \
		config.o	\
		trace.o		\
		lab2.o -o lab2

#============================================================ CLEAN

clean:
	rm -rf *.o
	rm -f lab2
