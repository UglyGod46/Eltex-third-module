CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =
SOURCES = sender.c receiver.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLES = sender receiver

all: $(EXECUTABLES)

sender: sender.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

receiver: receiver.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLES)

run:
	./receiver &
	sleep 1
	./sender
