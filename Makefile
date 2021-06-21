CC=gcc
CFLAGS=-g -o
TARGETS=scheduler
all: $(TARGETS)
.PHONY: all
%:
	$(CC) $(CFLAGS) $@ $@.c
       
clean:
	rm $(TARGETS)