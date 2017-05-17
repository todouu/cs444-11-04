#ifndef concurrency
#define concurrency

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <immintrin.h>
#include <stdio.h>
#include <signal.h>


struct mylink{
	int value;
	link *next;
};

void *searcher(void* );
void *inserter(void* );
void *deleter(void* );

void freeBuffer();
void run_handler(int );

//linked list function

mylink find_last(mylink*);



#endif
