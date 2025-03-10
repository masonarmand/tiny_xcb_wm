CC = gcc
CFLAGS = -std=c89
LDFLAGS = -lxcb -lxcb-util -lxcb-keysyms
SOURCES = *.c
EXEC = tinyxcb

.PHONY: all clean

all: build

build:
	$(CC) -g $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXEC)
run:
	./$(EXEC)
clean:
	-rm $(EXEC)
