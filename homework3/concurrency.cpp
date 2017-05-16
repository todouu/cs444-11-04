/* 
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman 
 * Class: CS 444 - Operating Systems II
 * Date: 5/16/17
 * Description: Categorical Mutual Exclusion
 */

#include "concurrency.hpp"
#include <immintrin.h>
#include <stdio.h>
#include <signal.h>

#define SUCCESS 1
#define RETRY_LIMIT_EXCEEDED 0
#define RETRY_LIMIT 10
#define MaxThreadNum 4
#define MaxJob 32
static volatile int run = 1;

//phtread varb
pthread_mutex_t mutexnum;
pthread_mutex_t cmutexnum; // conusmer mutex 
pthread_mutex_t pmutexnum; // producer mutex
pthread_cond_t c_empty;
pthread_cond_t c_full;

mydata* work[MaxJob];
int work_count;

void run_handler(int val) 
{
        run = 0;
}


void *searcher(void* data)
{
        while(1) {  
                //Lock out other consumer threads so they don't try and access same value
                pthread_mutex_lock(&cmutexnum);
                pthread_mutex_lock(&mutexnum);
                
                //create temp struct
                mydata *local = (mydata *) malloc(sizeof(mydata));
                
                //if is empty, just wait
                while(work_count == 0) {
                        pthread_cond_wait(&c_empty, &mutexnum);
                }
               
                //Pull struct from buffer
                local->waitnumer = work[work_count - 1]->waitnumer;
                local->worknumer = work[work_count - 1]->worknumer;
                free(work[work_count -1]);
                work_count--;

                //unlock the data
                pthread_mutex_unlock (&cmutexnum);
                pthread_mutex_unlock (&mutexnum);

                //let other thread know, u can work!
                if(work_count < MaxJob - 1) {
                        pthread_cond_broadcast(&c_full);
                }

                //sleep
                sleep(local->waitnumer);
                printf("The Consumer used this number: %d\n", local->worknumer); 

                free(local);
        }
}

void *inserter()  //void* data)
{
        while(1) {
                pthread_mutex_lock(&pmutexnum);
                pthread_mutex_lock(&mutexnum);
                
                //create local data 
                mydata *local = (mydata*) malloc(sizeof(mydata)); //(mydata *) data;
	
                //if work is full, just wait
                while(work_count == MaxJob) {
                        pthread_cond_wait(&c_full, &mutexnum);
                }

                //do work
                local->worknumer = my_rand(1);
                local->waitnumer = my_rand(2);
                work[work_count] = local;
                work_count++;

                //unlock the data
                pthread_mutex_unlock (&pmutexnum);
                pthread_mutex_unlock (&mutexnum);

                //let other thread know you can work!
                if(work_count) {
                        pthread_cond_broadcast(&c_empty);
                }

                sleep(my_rand(3));
        }
}

void *deleter(void *) 
{
}

// Free allocated space for the buffer and structs
void freeBuffer() 
{
        int i;
        for(i = 0; i < MaxJob; i++) {
                if(work[i] != NULL) {
                        free(work[i]);
                }
        }
}

int main (int argc, char **argv)
{
        srand (time (NULL));
        //create thread
        pthread_t threads[6];

        //Create the locks; one for the producer and consumer, and a general lock
        pthread_mutex_init(&mutexnum,NULL);
        pthread_mutex_init(&pmutexnum, NULL); // initialize producer mutex
        pthread_mutex_init(&cmutexnum, NULL); // initialize consumer mutex

        // Initialize our broadcast variables 
        pthread_cond_init(&c_full, NULL);
        pthread_cond_init(&c_empty, NULL);
        
        // Initialize our buffer count variable
        work_count = 0;
     
        // Generate two threads for the producer and two for the consumer
        pthread_create(&threads[0], NULL, searcher, NULL);
        pthread_create(&threads[1], NULL, searcher, NULL);
        pthread_create(&threads[2], NULL, inserter, NULL);
        pthread_create(&threads[3], NULL, inserter, NULL);        
        pthread_create(&threads[4], NULL, deleter, NULL);
        pthread_create(&threads[5], NULL, deleter, NULL);

        // Catch ctrl-c to start the end of process of the program
        signal(SIGINT, run_handler);

        while(run) {
                continue;
        }
        // kill threads
        int i;
        for (i = 0; i < 4; i++) {
                pthread_kill(threads[i], SIGALRM);
                pthread_join(threads[i], NULL);
        }

        // free memory from struct array
        freeBuffer();
       
        pthread_mutex_destroy (&mutexnum);
        pthread_mutex_destroy (&pmutexnum);
        pthread_mutex_destroy (&cmutexnum);

        return 0;
}
        
