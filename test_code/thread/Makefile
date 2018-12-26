OBJECTS=thread.o
RM=/bin/rm
CC=gcc
LD=gcc
CFLAGS=-D_GNU_SOURCE
LD_FLAGS=-lpthread

.PHONY: clean

thread: $(OBJECTS)
	$(LD) $(OBJECTS) $(LD_FLAGS) -o thread

*c.*o: 
	$(CC) $(CFLAGS) -o $@

clean: 
	$(RM) -rf $(OBJECTS) thread
