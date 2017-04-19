CFLAGS=-std=gnu11 -O2 -Wall -Werror
PROGRAMS=client server

all: $(PROGRAMS)

client: client.o common.o
	$(CC) $+ -o '$@'

server: server.o common.o
	$(CC) $+ -o '$@' -lbcc

clean:
	rm --force --verbose -- $(PROGRAMS) *.o

.PHONY: all clean
