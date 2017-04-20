#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
	int worknumer;
	int waitnumer;
} mydata;


void *consumer(void* data);
void *prodoucer(void* data);
int myrand(int cases);
