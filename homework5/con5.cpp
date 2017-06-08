 /* 
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman 
 * Class: CS 444 - Operating Systems II
 * Date: 06/06/17
 * Description: Cigarette smokers problem
 */

#include "concurrency.hpp"
#define MaxThreadNum 4

//enum means give number for each varb.
typedef enum{agent, smoker} op_t;
typedef enum{ACTIVE, DEACTIVE} state_t;

//semaphore varb
sem_t sem_agent;
sem_t tobacco;
sem_t paper;
sem_t match;

state_t agent_state = DEACTIVE;
state_t smoker_state = DEACTIVE;
int switcher(op_t op );

// Global Linked list init

void *agent(void* t)
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

void *smoker(void* t)  //void* data)
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

//This our own switcher
//
//##### NEEDS TO BE EDITED ############
//
int switcher(op_t op ){
	switch(op){
		case search:
			if(agent_state == DEACTIVE){
				sem_post(&sem_searcher);
				return 1;
			}
			break;
		case insert:
			if(smoker_state == DEACTIVE && inserter_state == DEACTIVE){
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
//       main function 
//=============================
int main (int argc, char **argv)
{
        //create thread
        pthread_t threads[MaxThreadNum];

        // init the sem varb
        sem_init(&sem_searcher, 0, 4);
        sem_init(&sem_inserter, 0, 3);
        sem_init(&sem_deleter, 0, 2);

        
        // Generate an agent thread and 3 smoker threads  
        // Agent thread
        pthread_create(&threads[0], NULL, agent, (void*) (intptr_t)1);
        //smoker threads
	pthread_create(&threads[1], NULL, smoker, (void*) (intptr_t)2);
       	pthread_create(&threads[2], NULL, smoker, (void*) (intptr_t)3);
	pthread_create(&threads[3], NULL, smoker, (void*) (intptr_t)4);

        return 0;
}
        
