CC 			= gcc
CFLAGS		= -Wall -g -v -std=c99
LIBRARIES 	= -lpthread -mrdrnd
DEPS		= concurrency1.h mt.h
OBJ			= concurrency1.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBRARIES) 
run: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBRARIES)

.DEFAULT_GOAL := run
.PHONY: clean

clean:
	rm *.o

