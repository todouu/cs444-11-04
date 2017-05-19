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

int try_insert = 0;
int try_delete = 0;

// Global Linked list init
struct node *head = new node;

void *searcher(void* t)
{
	int id = (intptr_t) t;
        while(1) {  
        	//========Class note============
			//searchSwitch.wait(noSearcher) 
			//critical section 
			//searchSwitch.signal(noSearcher)
			//==============================
            //Lock out other threads so they don't try and access same value
            //int x = //rand % size of linked list 
	    // loop until at x number in the linked list,
	    // print out the linked list value and thread id
	    // end
            cout << "Searcher, this is my id#: " << id << endl;   
            //unlock the data
        }
}

void *inserter(void* t)  //void* data)
{
	int id = (intptr_t) t;
        while(1) {

        	//========Class note============
			// insertSwitch.wait(noInserter) 
			// insertMutex.wait()
			// # critical section
			// insertMutex.signal()
			// insertSwitch.signal(noInserter)
			//==============================

        	//pthread_mutex_lock(&locker);
	    if(try_insert == 1) {
       	   	 pthread_cond_wait(&insert_free,&locker);
	    }
	    try_insert = 1;
           	//create local data 
            //mylink *head = new mylink; 
		
   	    cout << "Inserter, this is my id#: " << id << endl; 
            //unlock the data
            //pthread_mutex_unlock(&locker);

		    //let other thread know you can work!
            try_insert = 0;
	    pthread_cond_broadcast(&insert_free);

        }
}

void *deleter(void* t) 
{
	int id = (intptr_t) t; 	
	while(1) {
		cout << "Deleter before lock." << endl;
		pthread_mutex_lock(&locker);
		//int x = // rand % size of linked list
		cout << "Deleter, this is my id#: " << id << endl; 
		pthread_mutex_unlock(&locker); 
		cout << "Deleter unlocked." << endl;
		sleep(3);
	}
}

//=============================
//    linked list function
//=============================

//Create a new node
struct node* new_node(int x){
	struct node* newNode = new node; 
	newNode->data = x;
	newNode->next = NULL;
	return newNode;
}

//Insert data at the tail of linked list
void insert_end(int x) {
   	struct node* cur = head;
  	struct node* newNode = new_node(x);
	if(head == NULL) {
		head = newNode;
		return;
	}
	while(cur->next != NULL){ 
		cur = cur->next; 
		cur->next = newNode;
	}
}

//Delete a val from linked list
int delete_val(int x)
{
	struct node* cur = head;
	struct node* temp;
	//nothing need to delete
	if(head == NULL)
	{
		return -1;
	}
	//if only one things to delete
	if(head->data == x) {
		if(head->next != NULL)
		{
			temp = head->next;
		 	free(head);
			head = temp;
		}		
	    else{
			free(head);
			head = NULL;
		}
			return 0;
	}	
	//search val to delete
	while(cur->next != NULL){
			temp = cur;
			cur = cur->next;
			if(cur->data == x){
				temp->next = cur->next;
				free(cur);
				return 0;
			}
	}
	return -1;
}

//Search a value in the linked list
int search_val(int x)
{
	struct node* cur = head;
	if(head == NULL){
	   return -1;
	}
	while(cur->next != NULL){
		if(cur->data == x){
			return 0;
	  	}
		cur = cur->next;
	 }
	return -1;
}


// signal control
void run_handler(int val) 
{
        run = 0;
}

//=============================
//       main function 
//=============================
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
        
        // Generate two threads for each of them 
        // Searcher
        pthread_create(&threads[0], NULL, searcher, (void*) (intptr_t)1);
        pthread_create(&threads[1], NULL, searcher, (void*) (intptr_t)2);
        // Inserter
        pthread_create(&threads[2], NULL, inserter, (void*) (intptr_t)3);
        pthread_create(&threads[3], NULL, inserter, (void*) (intptr_t)4);        
        // Deleter
        pthread_create(&threads[4], NULL, deleter, (void*) (intptr_t)5);
        pthread_create(&threads[5], NULL, deleter, (void*) (intptr_t)6);

        // Catch ctrl-c to start the end of process of the program
        signal(SIGINT, run_handler);
        int count = 0;
        while(run) {
        		count++;
        		if (count == 100000){
        			 exit(0) ;
        		}
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
       
        pthread_mutex_destroy (&locker);

        return 0;
}
        
