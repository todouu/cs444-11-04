 /* 
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman 
 * Class: CS 444 - Operating Systems II
 * Date: 5/16/17
 * Description: Categorical Mutual Exclusion
 */

#include "concurrency.hpp"

#define MaxThreadNum 6
static volatile int run = 1;

//enum means give number for each varb.
typedef enum{search, insert, delete_op} op_t;
typedef enum{ACTIVE, DEACTIVE} state_t;

//semaphore varb
sem_t sem_searcher;
sem_t sem_inserter;
sem_t sem_deleter;

int in_searchers = 0;
state_t inserter_state = DEACTIVE;
state_t deleter_state = DEACTIVE;
int switcher(op_t op );

// Global Linked list init
struct node *head = new node;

void *searcher(void* t)
{
	int id = (intptr_t) t;
	//random seed setup
	srand (time(NULL));
        while(1) {  
        	sleep(1);
        	// check if a deleter is working
        	if(deleter_state == ACTIVE){
        		cout << "Searcher <" << id << "> wait for deleter finishing job" << endl;
        	}
        	sem_wait(&sem_searcher);
        	cout << "Searcher <" << id << "> ready to search " << endl;
        	in_searchers++;

        	// Link list part
        	// generate a random number between 1 to 10;
        	int rnd = rand() %10 + 1 ; 
        	if (search_val(rnd) == 0){
        		cout << "Searcher <" << id << "> found {" << rnd << "} in the list" << endl;
        		cout << endl;
        	}
        	else{
        		cout << "Searcher <" << id << "> Opps {" << rnd << "} is not in the list" << endl;
        		cout << endl;
        	}

        	//after search finish job, decrease the in_sercher
        	in_searchers--;
        	//check if delete is working
        	//then continue search
        	if (switcher(delete_op) == 0){
        		switcher(search);
        	}
        }
}

void *inserter(void* t)  //void* data)
{
	int id = (intptr_t) t;
	//random seed setup
	srand (time(NULL));
        while(1) {
        	sleep(1);
        	// check if insert is working or another deleter working
	    	if(deleter_state == ACTIVE || inserter_state == ACTIVE) {
	    		cout << "Inserter <" << id << "> have to wait for deleter or another inserter" << endl;
	    	}
	    	sem_wait(&sem_inserter);
	    	inserter_state = ACTIVE;

	    	// Link list part
        	// generate a random number between 1 to 100;
	    	int rnd = rand() % 10 + 1;
	    	cout << "Inserter <" << id << "> insert a number {" << rnd << "} to the end of the list" << endl;
	    	insert_end(rnd);
	    	// Done with link list part
	    	//cout << "Inserter <" << id << "> have inserted a number" << endl <<endl;

	    	inserter_state = DEACTIVE;
	    	if(switcher(delete_op) == 0){
	    		switcher(insert);
	    	}

        }
}

void *deleter(void* t) 
{
	int id = (intptr_t) t; 	
	//random seed setup
	srand (time(NULL));
	while(1) {
        sleep(2);
		if(in_searchers > 0 || inserter_state == ACTIVE){
			cout << "Deleter <" << id << "> has to wait for insert finishing job" << endl;
		}
		sem_wait(&sem_deleter);
		deleter_state = ACTIVE;

		// Link list part
        // generate a random number between 1 to 100;
	    int rnd = rand() % 10 + 1;
	    cout << "Deleter <" << id << "> try delete number {" << rnd << "} from the list" << endl;
	    if (delete_val(rnd) == 0){
	    	cout << "Deleter <" << id << "> delete success | number =" << rnd << endl;
        	cout << endl;
	    }
	    else{
	    	cout << "Deleter <" << id << "> fail to delete a number" << endl;
        	cout << endl;
	    }
	    //Done with link list

	    deleter_state = DEACTIVE;
	    //check if safe to insert and search
	    switcher(insert);
	    switcher(search);

	}
}


//This our own switcher
int switcher(op_t op ){
	switch(op){
		case search:
			if(deleter_state == DEACTIVE){
				sem_post(&sem_searcher);
				return 1;
			}
			break;
		case insert:
			if(deleter_state == DEACTIVE && inserter_state == DEACTIVE){
				sem_post(&sem_inserter);
				return 1;
			}
			break;
		case delete_op:
			if(inserter_state == DEACTIVE && in_searchers == 0){
				sem_post(&sem_deleter);
				return 1;
			}
			break;

	}
	return 0;
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
	int temp = 1;
	if(head == NULL) {
		head = newNode;
		return;
	}
	while(temp) {
		if(cur->next == NULL) { 
		        cur->next = newNode;
		        temp = 0;	
		}
		cur = cur->next; 
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
        int i;

        //create thread
        pthread_t threads[MaxThreadNum];

        // init the sem varb
        sem_init(&sem_searcher, 0, 4);
        sem_init(&sem_inserter, 0, 3);
        sem_init(&sem_deleter, 0, 2);

        
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
        //signal(SIGINT, run_handler);
        //while(run) {
        //        continue;
        //}
		// joining
        for (i = 0; i < MaxThreadNum; i++) {
                pthread_join(threads[i], NULL);
        }
        // kill threads
		// for (i = 0; i < MaxThreadNum; i++) {
        //        pthread_kill(threads[i], SIGALRM);
       // }

        // free memory from struct array
        // freeBuffer();
       

        return 0;
}
        
