CC 			= gcc
CFLAGS		= -Wall -g -v -std=c11
LIBRARIES 	= -lpthread
DEPS		= concurrency1.h c_stack.h
OBJ			= concurrency1.o c_stack.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBRARIES) 
run: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBRARIES)

.DEFAULT_GOAL := run
.PHONY: clean

clean:
	rm *.o

