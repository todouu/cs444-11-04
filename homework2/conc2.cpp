/*
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman
 * Class: CS 444 - Operating Systems II
 * Date: May 5th, 2017
 * Description: The Dining Philosophers Problem
 */

// Libraries
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>
#include <cstdint>

// Definitions
#define MAX_FORKS 5
#define MAX_PHILOSOPHERS 5

using namespace std;

// Global Variables
pthread_mutex_t mutexnum;
pthread_cond_t fork_empty; // Broadcast variable to let other threads know when to use forks


/*
 * Fork struct
 * Inormation: -Keeps track of whether the fork is 
 *             in use or not.
 */
struct var_frk 
{
	//bool in_use = false; // false meaning the fork is not in use
	bool in_use ; // false meaning the fork is not in use
};

/*
 * Philosopher struct
 * Information: -Keeps track of their left and right fork and their name
 */
struct philosopher
{
	int left;
	int right;
	string name;
	bool getting_left_frk; 
	bool getting_right_frk;
};

/*
 * thread_data
 * this struct for passing data to thread
 */

struct thread_data
{
	int tid;
	var_frk *thread_frk;
	philosopher *thread_phi;

};



/*
 * Think function
 * Information: -Calculates random number between 1-20
 * 		-Thread prints it's # and think time (the rand number)
 * 		-Think sleeps for think time (the rand number)
 */
void think(int x, philosopher *arr_phil)
{
	int rand_var = rand() % 20 + 1; // rand range 1-20 seconds;
	
	// PRINT "Thread -insert number- is sleeping for rand_var
	printf("%s is thinking for %d seconds\n", arr_phil[x].name.c_str(), rand_var);
	sleep(rand_var);
}

/*
 * Eat function
 * Information: -Calculates random number between 2-9 seconds
 * 		-Thread prints it's # and eat time (the rand number)
 * 		-Eat sleeps for eat time (the rand number)
 */
void eat(int x,  philosopher *arr_phil ) 
{
	int rand_var = rand() % 9 + 2; // rand range for 2-9 seconds;

	//PRINT "Thread -insert thread number- is eating for rand_var
	printf("%s is eating for %d seconds\n", arr_phil[x].name.c_str(), rand_var);
	sleep(rand_var);
}

/*
 id* Get fork function
 * Information: -Lets a thread pick up a left and right fork if avaialable so it can eat
 */
int get_fork(int id, philosopher* me, var_frk *afrk) 
{

	printf("%s get a fork. \n", me[id].name.c_str());	
	// letting neighbors know that we are grabbing the forks
	me[id].getting_right_frk = true;
	me[id].getting_left_frk = true;

	// check if left and right hand are in use
	if (afrk[ me[id].left ].in_use == true || afrk[ me[id].right ].in_use == true){
		puts("some one used fork.");
		me[id].getting_right_frk = false;
		me[id].getting_left_frk = false;
		return 0;
	}
	else{
		puts("yeah~ ");
		afrk[ me[id].left ].in_use = true;
		afrk[ me[id].right].in_use = true;
		me[id].getting_right_frk = false;
		me[id].getting_left_frk = false;
		return 1;
	}
}

/*
 * Put fork function
 * Information: -Thread/philosopher puts forks back down
 */
void put_fork (int id, var_frk *afrk, philosopher *me) 
{
	printf("%s put a fork. \n", me[id].name.c_str());	
	printf("\n \n");
	afrk[ me[id].left ].in_use = false;
	afrk[ me[id].right].in_use = false;
}

int check_neighbor(int id, philosopher *me){
	int neighbor_left ; 
	int	neighbor_right ;

	if (id - 1 < 0){
		id = 4;
		neighbor_left = id;
	}
	else{
		neighbor_left = id;
	}

	if (id + 1 > 4){
		id = 0;
		neighbor_right= id;
	}
	else{
		neighbor_right= id;
	}
	
	if( me[neighbor_left].getting_right_frk == true || me[neighbor_right].getting_left_frk== true )
	{
		return 0;
	}
	else{
		return 1;
	}

}




void *dinner_time(void *thread_data) 
{
	struct thread_data *mydata;
	mydata = (struct thread_data *) thread_data;
	int id = mydata->tid;
	var_frk *local_frk = mydata->thread_frk;
	philosopher *local_phi = mydata->thread_phi;
	

	while(true) {
		think(id, local_phi);
		//check neighbors hands
		if(check_neighbor(id,local_phi))
		{
			//check my hands
			if( get_fork(id, local_phi, local_frk) != 0)
			{
				eat(id, local_phi);
				put_fork(id, local_frk, local_phi);
			}
		}
	}
}

/*
 * Main function
 * Information: -Intializes Threads
 * 		-Exectues program functionality
 * 		-Garbage collection on program termination
 */
int main () 
{	
	void *status;
	int i;
	var_frk *arr_fork = new var_frk[MAX_FORKS];
	//philosopher *arr_phil = new philosopher[MAX_PHILOSOPHERS];
	thread_data thread_data_array;

	// Seed random number generator
	srand (time (NULL)); 	
  
	// initialize the array of fork and philosophs 
	for (i = 0; i < MAX_FORKS; i++){
		arr_fork[i].in_use = false;
	}

	philosopher arr_phil[] = {{4, 0, "Edsger Dijkstra", false, false}, {0, 1, "Harrison Ford", false, false},
	   					{1, 2, "Will Smith", false, false}, {2, 3, "Gottfried Leibniz", false, false}, {3, 4, "Garth Brooks", false, false}}; 


	//Creates the five threads
    pthread_t threads[MAX_PHILOSOPHERS]; // C Threads

    //Create a lock
    pthread_mutex_init(&mutexnum, NULL);

    //Create our broadcast variables
	pthread_cond_init(&fork_empty, NULL);

	/* Initialize and set thread detached attribute */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	//debug
	for(int i = 0; i < MAX_PHILOSOPHERS ; i++){
		cout << "name: " << arr_phil[i].name << endl;
		cout << "left: " << arr_phil[i].left<< endl;
		cout << "right: " << arr_phil[i].right<< endl;
	}

	thread_data_array.thread_frk = arr_fork; 
	thread_data_array.thread_phi = arr_phil;
	
	// generate threads 
	for(int i = 0; i < MAX_PHILOSOPHERS ; i++){
		thread_data_array.tid = i;
		pthread_create(&threads[i], &attr, dinner_time, (void*) &thread_data_array);
	}	

	
	// Kill threads
	for (int i = 0; i < MAX_PHILOSOPHERS; i++) {
	//	pthread_kill(threads[i],run);
		pthread_join(threads[i],&status);
	}

	pthread_mutex_destroy (&mutexnum);
	
	return 0;		
}


