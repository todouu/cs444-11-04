/*
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman
 * Class: CS 444 - Operating Systems II
 * Date: May 5th, 2017
 * Description: The Dining Philosophers Problem
 */

using namespace std;

// Libraries
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

// Definitions
#define MAX_FORKS 5
#define MAX_PHILOSOPHERS 5

// Global Variables
pthread_mutex_t mutexnum;
pthread_cond_t fork_empty; // Broadcast variable to let other threads know when to use forks
var_frk **arr_fork[];
philosopher **arr_phil[];

void _initialize (void) 
{
	// Initialize fork array
	arr_fork = malloc(MAX_FORKS * sizeof (*arr_fork));
	int i;
	for (i = 0; i < MAX_FORK; i++) {
		arr_fork[i] = malloc (sizeof (struct var_frk));
		arr_fork[i]->in_use = false;
	}
	
	// Initialize philosopher array
	arr_phil = malloc (MAX_PHILOSOPHERS * sizeof (*arr_phil));
	for (i = 0; i < MAX_PHILOSOPHERS; i++) {
		arr_phil[i] = malloc (sizeof (struct philosopher));
	}
	arr_phil[];
}

var_frk* arr_fork[MAX_FORK] = {{false}, {false}, {false}, {false}, {false}}; // Array of forks, keeps track of all the forks
philosopher* arr_phil[MAX_PHILOSOPHER] = {{4, 0, "Edsger Dijkstra"}, {0, 1, "Harrison Ford"}, {1, 2, "Will Smith"}, {2, 3, "Gottfried Leibniz"}, {3, 4, "Garth Brooks"}}; 
static volatile int run = 1;

// handle program termination
void run_handler (int val)
{
	run = 0;
}

/*
 * Fork struct
 * Inormation: -Keeps track of whether the fork is 
 *             in use or not.
 */
struct var_frk 
{
	bool in_use = false; // false meaning the fork is not in use
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
};

/*
 * Think function
 * Information: -Calculates random number between 1-20
 * 		-Thread prints it's # and think time (the rand number)
 * 		-Think sleeps for think time (the rand number)
 */
void think(int x)
{
	int rand_var = rand() % 20 + 1; // rand range 1-20 seconds;
	
	// PRINT "Thread -insert number- is sleeping for rand_var
	printf("%s is thinking for %d seconds\n", arr_phil[x]->name, rand_var);
	sleep(rand_var);
}

/*
 * Eat function
 * Information: -Calculates random number between 2-9 seconds
 * 		-Thread prints it's # and eat time (the rand number)
 * 		-Eat sleeps for eat time (the rand number)
 */
void eat(int x) 
{
	int rand_var = rand() % 9 + 2; // rand range for 2-9 seconds;

	//PRINT "Thread -insert thread number- is eating for rand_var
	printf("%s is eating for %d seconds\n", arr_phil[x]->name, rand_var);
	sleep(rand_var);
}

/*
 * Get fork function
 * Information: -Lets a thread pick up a left and right fork if avaialable so it can eat
 */
void get_fork() 
{
	printf("I get a fork. \n");	
}

/*
 * Put fork function
 * Information: -Thread/philosopher puts forks back down
 */
void put_fork () 
{
	printf("I put a fork. \n");	
}

void *dinner_time(int x) 
{
	while(true) {
		think(x);
		get_fork();
		eat(x);
		put_fork();
	}
}

/*
 * Main function
 * Information: -Intializes Threads
 * 		-Exectues program functionality
 * 		-Garbage collection on program termination
 */
int main () 
{	// Seed random number generator
	srand (time (NULL)); 	
      
	// initialize the array of fork and philosophs 
	_initialize(); 

	//Creates the five threads
    pthread_t threads[MAX_PHILOSOPHERS]; // C Threads

    //Create a lock
    pthread_mutex_init(&mutexnum, NULL);

    //Create our broadcast variables
	pthread_cond_init(&fork_empty, NULL);
	
	// generate threads 
	for(int i = 0; i < MAX_PHILOSOPHERS ; i++){
		pthread_create(&threads[i], NULL, dinner_time, i);
	}	

	// Catch ctrl-c to start program termination
	signal(SIGINT, run_handler);

	while (run) {
		continue;
	}
	
	// Kill threads
	int i;
	for (i = 0; i < MAX_PHILOSOPHERS; i++) {
		pthread_kill(threads[i]);
		pthread_join(threads[i]);
	}

	pthread_mutex_destroy (&mutexnum);
	
	return 0;		
}


