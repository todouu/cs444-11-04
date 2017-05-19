#ifndef concurrency
#define concurrency

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <immintrin.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

struct mylink{
	int value;
	struct mylink *next;
};

void *searcher(void* );
void *inserter(void* );
void *deleter(void* );

void freeBuffer();
void run_handler(int );

//linked list function

mylink* find_last(mylink*);



#endif
