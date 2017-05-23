#ifndef concurrency
#define concurrency

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <immintrin.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

struct node{
	int data;
	struct node *next;
};

struct node* new_node(int );

void *searcher(void* );
void *inserter(void* );
void *deleter(void* );

void run_handler(int );

//linked list function
struct node* find_last(node*);
int search_val(int );
int delete_val(int );
void insert_end(int );



#endif
