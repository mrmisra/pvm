.PHONY: all clean

all: pvm

CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Isrc/headers
LDFLAGS=-lz
OBJS=main.o src/commands/util.o src/commands/init.o src/commands/save.o src/commands/commit.o src/commands/branch.o src/commands/history.o

pvm: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.c src/headers/init.h src/headers/save.h src/headers/commit.h src/headers/branch.h src/headers/history.h
src/commands/util.o: src/commands/util.c src/headers/util.h
src/commands/init.o: src/commands/init.c src/headers/init.h src/headers/util.h
src/commands/save.o: src/commands/save.c src/headers/save.h src/headers/util.h
src/commands/commit.o: src/commands/commit.c src/headers/commit.h src/headers/util.h
src/commands/branch.o: src/commands/branch.c src/headers/branch.h src/headers/util.h
src/commands/history.o: src/commands/history.c src/headers/history.h

clean:
	rm -f *.o src/commands/*.o pvm
