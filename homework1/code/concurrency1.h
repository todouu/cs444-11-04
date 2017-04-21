#ifndef concurrency1
#define concurrency1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
	int worknumer;
	int waitnumer;
} mydata;


void *consumer(void* data);
void *prodoucer(void* data);
int myrand(int cases);
int check_method();

#endif
                    