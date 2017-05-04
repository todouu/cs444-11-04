/*
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman
 * Class: CS 444 - Operating Systems II
 * Date: May 5th, 2017
 * Description: The Dining Philosophers Problem
 */

// Libraries
#include "conc2.hpp" 

// Definitions
#define MAX_FORKS 5
#define MAX_PHILOSOPHERS 5

// Global Variables
pthread_mutex_t mutexnum;
pthread_cond_t fork_empty;


var_frk arr_fork[]= {{false}, {false}, {false}, {false}, {false} };
philosopher arr_phil[] = {{4, 0, "Edsger Dijkstra", false, false, 0}, 
		    {0, 1, "Harrison Ford", false, false, 0},
	   	    {1, 2, "Will Smith", false, false, 0}, 
		    {2, 3, "Gottfried Leibniz", false, false, 0}, 
		    {3, 4, "Garth Brooks", false, false, 0}};  


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
	printf("ID:%d, %s is thinking for %d seconds\n", x ,arr_phil[x].name.c_str(), rand_var);
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
	printf("ID:%d, %s is eating for %d seconds\n", x ,arr_phil[x].name.c_str(), rand_var);
	sleep(rand_var);
}

/*
 id* Get fork function
 * Information: -Lets a thread pick up a left and right fork if avaialable so it can eat
 */
int get_fork(int id) 
{
	// letting neighbors know that we are grabbing the forks
	arr_phil[id].getting_right_frk = true;
	arr_phil[id].getting_left_frk = true;

	// check if left and right hand are in use
	if (arr_fork[ arr_phil[id].left ].in_use == true || arr_fork[ arr_phil[id].right ].in_use == true){
		printf("ID:%d, %s cannot grab a fork.\n", id, arr_phil[id].name.c_str());
		puts("==========Lose============");
		arr_phil[id].getting_right_frk = false;
		arr_phil[id].getting_left_frk = false;
		return 0;
	}
	else{
		printf("ID:%d, %s has the forks.\n", id, arr_phil[id].name.c_str());
		arr_fork[ arr_phil[id].left ].in_use = true;
		arr_fork[ arr_phil[id].right].in_use = true;
		arr_phil[id].getting_right_frk = false;
		arr_phil[id].getting_left_frk = false;
		arr_phil[id].count++;
		return 1;
	}
}

/*
 * Put fork function
 * Information: -Thread/philosopher puts forks back down
 */
void put_fork (int id) 
{
	printf("ID:%d, %s is putting the fork down.\n\n", id, arr_phil[id].name.c_str());	
	arr_fork[arr_phil[id].left].in_use = false;
	arr_fork[arr_phil[id].right].in_use = false;
}

int check_neighbor(int id ){
	int neighbor_left; 
	int neighbor_right;

	if (id - 1 < 0) {
		id = 4;
		neighbor_left = id;
	}
	else {
		neighbor_left = id;
	}

	if (id + 1 > 4) {
		id = 0;
		neighbor_right= id;
	}
	else {
		neighbor_right= id;
	}
	
	if (arr_phil[neighbor_left].getting_right_frk == true || arr_phil[neighbor_right].getting_left_frk == true)
	{
		return 0;
	}
	else{
		return 1;
	}

}

void *dinner_time(void *i) 
{
	int id = (intptr_t) i;
	//cout << "My ID is: " << id  << endl;

	while(true)
	{
		think(id);
		//check neighbors hands
		if(check_neighbor(id) != 0)
		{
			//check my hands
			if( get_fork(id) != 0)
			{
				eat(id);
				put_fork(id);
				//print out the total number
				puts("-------------");
				for(int j = 0; j < 5 ; j++){
					cout << "number: " << j << "total take forks #:" << arr_phil[j].count << endl;
				}
				puts("-------------");
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

	// Seed random number generator
	srand (time (NULL)); 	
  
	//Creates the five threads
    pthread_t threads[MAX_PHILOSOPHERS]; // C Threads

    //Create a lock
   	pthread_mutex_init(&mutexnum, NULL);

	/* Initialize and set thread detached attribute */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//debug
	for(i = 0; i < MAX_PHILOSOPHERS ; i++){
		cout << "name: " << arr_phil[i].name << endl;
		cout << "left: " << arr_phil[i].left<< endl;
		cout << "right: " << arr_phil[i].right<< endl;
	}


	
	// generate threads 
	for(i = 0; i < MAX_PHILOSOPHERS ; i++){
		pthread_create(&threads[i], &attr, dinner_time, (void*) (intptr_t) i);
	}	
	
	// Kill threads
	for (i = 0; i < MAX_PHILOSOPHERS; i++) {
		pthread_join(threads[i],&status);
	}

	printf("After joining \n");	
	//pthread_mutex_destroy (&mutexnum);
	
	return 0;		
}


