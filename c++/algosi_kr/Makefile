CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2

BIN=upcopychars upcopychars2 max_input spl_stty watchtty fillqueue

all: $(BIN)

upcopychars: upcopychars.c
	$(CC) $(CFLAGS) $< -o $@

upcopychars2: upcopychars2.c
	$(CC) $(CFLAGS) $< -o $@

max_input: max_input.c
	$(CC) $(CFLAGS) $< -o $@

spl_stty: spl_stty.c
	$(CC) $(CFLAGS) $< -o $@

watchtty: watchtty.c
	$(CC) $(CFLAGS) $< -o $@

fillqueue: fillqueue.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN)
