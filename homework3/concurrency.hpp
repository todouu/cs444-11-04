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
	int x;
	link *next;
};

void *searcher(void* );
void *inserter(void* );
void *deleter(void* );

#endif
