/* 
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman 
 * Class: CS 444 - Operating Systems II
 * Date: 5/16/17
 * Description: Categorical Mutual Exclusion
 */

#include "concurrency.hpp"

#define SUCCESS 1
#define RETRY_LIMIT_EXCEEDED 0
#define RETRY_LIMIT 10
#define MaxThreadNum 6
static volatile int run = 1;

//phtread varb
pthread_mutex_t locker;
pthread_cond_t delete_free;
pthread_cond_t delete_in_use;
pthread_cond_t insert_free;
pthread_cond_t insert_in_use;

//mydata* work[MaxJob];
int try_insert = 0;
int try_delete = 0;

// Global Linked list init
mylink *root = new mylink;
root->next = NULL;
root->value = 0;

void *searcher(void* id)
{
        while(1) {  
				//searchSwitch.wait(noSearcher) 

				//# critical section 

				//searchSwitch.signal(noSearcher)


                //Lock out other threads so they don't try and access same value
                pthread_mutex_lock(&cmutexnum);
                pthread_mutex_lock(&mutexnum);
               
                //if is empty, just wait
                pthread_cond_wait(&c_empty, &mutexnum);
               

                //unlock the data
                pthread_mutex_unlock (&cmutexnum);
                pthread_mutex_unlock (&mutexnum);

                //let other thread know, u can work!
                pthread_cond_broadcast(&c_full);

        }
}

void *inserter(void* id)  //void* data)
{
        while(1) {
		// #### NOTE
		// Do we need these locks? Delete should be the only one with locks
              	//pthread_mutex_lock(&pmutexnum);
             	//pthread_mutex_lock(&mutexnum);
        // #### NOTE

                //create local data 
                mylink *local = (mylink*) malloc(sizeof(mylink)); //(mydata *) data;
	
                //if try_search == 1, then another thread is trying to search
                while(try_insert == 1) {
                        pthread_cond_wait(&insert_in_use, &mutexnum);
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
                        pthread_cond_broadcast(&insert_free);
                }

                //sleep(my_rand(3));
        }
}

void *deleter(void* id) 
{

}


//linked list function
mylink find_last(mylink* head){
	head = root;
	while(head->next!= NULL){
		head = head->next 
	}
	return head;
}



// Free allocated space for the buffer and structs
void freeBuffer() 
{
	/*
        int i;
        for(i = 0; i < MaxJob; i++) {
                if(work[i] != NULL) {
                        free(work[i]);
                }
        }
    */
}

void run_handler(int val) 
{
        run = 0;
}


int main (int argc, char **argv)
{
        srand (time (NULL));
        //create thread
        pthread_t threads[MaxThreadNum];

        //Create the locks; one for the producer and consumer, and a general lock
        pthread_mutex_init(&locker,NULL);

        // Initialize our broadcast variables 
        pthread_cond_init(&insert_in_use, NULL); // Insert broadcast Variable
        pthread_cond_init(&insert_free, NULL); // Insert broadcast variable
        pthread_cond_init(&delete_in_use, NULL); // Delete broadcast variable
        pthread_cond_init(&delete_free, NULL); // Delete broadcast variable
        
        // Initialize our buffer count variable
        work_count = 0;
     
        // Generate two threads for each of them 
        // Searcher
        pthread_create(&threads[0], NULL, searcher, 1);
        pthread_create(&threads[1], NULL, searcher, 2);
        // Inserter
        pthread_create(&threads[2], NULL, inserter, 3);
        pthread_create(&threads[3], NULL, inserter, 4);        
        // Deleter
        pthread_create(&threads[4], NULL, deleter, 5);
        pthread_create(&threads[5], NULL, deleter, 6);

        // Catch ctrl-c to start the end of process of the program
        signal(SIGINT, run_handler);

        while(run) {
                continue;
        }
        // kill threads
        int i;
        for (i = 0; i < MaxThreadNum; i++) {
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
        
