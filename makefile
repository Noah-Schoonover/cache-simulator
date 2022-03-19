CC=gcc
CFLAGS=-g -Wall
CFLAGS_OBJ=-g -Wall -c

all: lab2

#============================================================ lab2

config.o: config.h config.c
	$(CC) $(CFLAGS_OBJ) config.c

trace.o: trace.h trace.c
	$(CC) $(CFLAGS_OBJ) trace.c

cache.o: cache.h cache.c
	$(CC) $(CFLAGS_OBJ) cache.c

util.o: util.h util.c
	$(CC) $(CFLAGS_OBJ) util.c

log.o: log.h log.c
	$(CC) $(CFLAGS_OBJ) log.c

lab2: lab2.c config.o trace.o cache.o util.o log.o
	$(CC) $(CFLAGS_OBJ) lab2.c
	$(CC) $(CFLAGS) \
		config.o	\
		trace.o		\
		cache.o		\
		util.o		\
		log.o		\
		lab2.o -o lab2

#============================================================ CLEAN

clean:
	rm -rf *.o
	rm -f lab2
