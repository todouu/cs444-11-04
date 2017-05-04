#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <unistd.h>

using namespace std;

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



void think(int x, philosopher *arr_phil);
void eat(int x,  philosopher *arr_phil );
int get_fork(int id, philosopher* me, var_frk *afrk);
void put_fork (int id, var_frk *afrk, philosopher *me);
int check_neighbor(int id, philosopher *me);
void *dinner_time(void *i);
