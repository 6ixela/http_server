CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -pthread
LDLIBS = `pkg-config --libs glib-2.0`

EXE = multi_threaded

all: ${EXE}

multi_threaded: multi_threaded.o queue.o shared_queue.o

shared_queue.o: shared_queue.c shared_queue.h queue.o
queue.o: queue.c queue.h

.PHONY: clean

clean:
	${RM} *.o
	${RM} ${EXE}