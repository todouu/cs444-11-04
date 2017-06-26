#ifndef concurrency
#define concurrency

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <immintrin.h>
#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

void *agent(void* );
void *smoker(void* );

#endif
