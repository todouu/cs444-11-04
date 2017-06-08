/* 
* Programmers: Andrew Bowers, Shuai Peng 
* Class: CS 444 - Operating Systems II
* Date: 06/06/17
* Description: Cigarette smokers problem
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

// Declare semaphores.
sem_t agentSem;
sem_t tobacco, paper, matches;
sem_t tobaccosem, papersem, matchessem, mutex;
bool isTobacco = false;
bool isPaper = false;
bool isMatch = false;

// smoker with tobacco.
void* smokerA(void*) {	
	while(1) {
		sem_wait(&tobaccosem);
		printf("Smoker A will start smoking.\n");
		sleep(rand() % 3 + 1);
		printf("Smoker A has finished smoking.\n\n");
		sem_post(&agentSem);
	}
}

// smoker with paper.
void* smokerB(void*) {
	while(1) {
		sem_wait(&papersem);
		printf("Smoker B will start smoking.\n");
		sleep(rand() % 3 + 1); 
		printf("Smoker B has finished smoking.\n\n");
		sem_post(&agentSem);
	}
}

// smoker with matches.
void* smokerC(void*) {
	while(1) {
		sem_wait(&matchessem);
		printf("Smoker C will start smoking.\n");
		sleep(rand() % 3 + 1);
		printf("Smoker C has finished smoking.\n\n");
		sem_post(&agentSem);
	}
}

// the agent that places items down. 
void* agentFunc(void*) {
	while(1) {
		sem_wait(&agentSem);		
		int items = rand() % 3;
		sleep(rand() % 5 + 1);
		switch(items) {
			case 0 : sem_post(&paper); 
					 sem_post(&matches); // Items are paper and matches.
					 printf("Agent are placed paper and matches.\n");
					 break;
			case 1 : sem_post(&tobacco); 
					 sem_post(&matches);// Items are tobacco and matches.
				 	 printf("Agent are placed tobacco and matches.\n");
					 break;
			case 2 : sem_post(&tobacco); 
					 sem_post(&paper);// Items are tobacco and paper.
					 printf("Agent are placed tobacco and paper.\n");
					 break;
		}
	}
}

/*This pusher prevents deadlocks for tobacco*/
void* tobacco_pusher(void*){
	while(1){
	    printf("Tobacco pusher wait for tobacco \n");
	    sem_wait(&tobacco);
	    printf("tobacco pusher get tobacco from the table \n");
	    sem_wait(&mutex);
	    if(isPaper){
	      isPaper = false;
	      sem_post(&matchessem);
	      printf("tobacco puhser give the tobacco and paper to smoker C\n");
	    }
	    else if(isMatch){
	      isMatch = false;
	      sem_post(&papersem);
	      printf("tobacco puhser give the tobacco and matches to smoker B\n");
	    }
	    else {
	    	isTobacco = true;
	      	printf("tobacco puhser find nothing, and put tobacco back\n");

	    }
	    sem_post(&mutex);
  		}
}


/*This pusher prevents deadlocks for paper*/
void* paper_pusher(void*){
  while(1){
  		printf("paper_pusher wait for paper \n");	
		sem_wait(&paper);
		printf("paper_pusher get the paper from the table \n");
		sem_wait(&mutex);
		if(isTobacco){
		  isTobacco = false;
		  sem_post(&matchessem);
		  printf("paper_pusher give the paper and tobacco to smoker C\n");
		}
		else if(isMatch){
		  isMatch = false;
		  sem_post(&tobaccosem);
		  printf("paper_pusher give the paper and mathces to smoker A\n");
		}
		else {
			isPaper = true;
			printf("paper_pusher find nothing from table, and put paper back \n");
		}
		sem_post(&mutex);
		}
}

/*This pusher prevents deadlocks for match*/
void* matches_pusher(void*){
  while(1){
  		printf("matches_pusher wait for matches \n");
	    sem_wait(&matches);
	    printf("matches_pusher get the matches from table \n");
	    sem_wait(&mutex);
	    if(isTobacco){
	      isTobacco = false;
	      sem_post(&papersem);
	      printf("matches_pusher give the mathces and tobacco to somker B\n");
	    }
	    else if(isPaper){
	      isPaper = false;
	      sem_post(&tobaccosem);
	      printf("matches_pusher give the mathces and paper to somker A\n");
	    }
	    else{ 
	    	isMatch = true;
			printf("matches_pusher find nothing from table, and put ma back \n");
	    }
	    sem_post(&mutex);
	    }
}


int main() {
	
	srand(time(NULL));
	// Initialize the semaphores/
	sem_init(&agentSem, 0, 1);
	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&matches, 0, 0);
	sem_init(&papersem, 0, 0);
	sem_init(&tobaccosem, 0, 0);
	sem_init(&matchessem, 0, 0);
	sem_init(&mutex, 0, 1);

	// Declares the pthreads.
	pthread_t smoker[2];
	pthread_t pusher[2];
	pthread_t agent;

	// Creates the 3 smoker threads.
	pthread_create(&smoker[0], NULL, &smokerA, NULL);
	pthread_create(&smoker[1], NULL, &smokerB, NULL);
	pthread_create(&smoker[2], NULL, &smokerC, NULL);
	// Creates the 3 pueser threads.
	pthread_create(&pusher[0], NULL, &tobacco_pusher, NULL);
	pthread_create(&pusher[1], NULL, &paper_pusher, NULL);
	pthread_create(&pusher[2], NULL, &matches_pusher, NULL);
	// The agent threads.	
	pthread_create(&agent, NULL, &agentFunc, NULL);
	for(int i=0 ; i<3 ; i++){
		pthread_join(smoker[i], NULL);
		pthread_join(pusher[i], NULL);
	}
	pthread_join(agent, NULL);

	std::cout << "GG" << std::endl;

}
