/* 
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman 
 * Class: CS 444 - Operating Systems II
 * Date: 4/19/17
 * Description: The producer-consumer problem
 */

#include "concurrency1.h"

#define MaxThreadNum 5

//phtread varb
pthread_t callThd[MaxThreadNum];
pthread_mutex_t mutexnum;

void *consumer(void* data){
	//create local data
	mydata *local_data;
	//point thie pointer to out-scope data
	local_data = data;

	pthread_mutex_lock (&mutexnum);
//	pop();
	pthread_mutex_unlock (&mutexnum);
	pthread_exit(NULL);
}

void *prodoucer(void* data){
	//create local data
	mydata *local_data;
	//point thie pointer to out-scope data
	local_data = data;

	pthread_mutex_lock (&mutexnum);
//	push(data);
	pthread_mutex_unlock (&mutexnum);
	pthread_exit(NULL);
}

int my_rand(int cases){
	int number;
	siwtch (cases){
		case '1':
			//any number we want
			//store data
			break;
		case '2':
			// 2 to 9 seconds
			// consumer wait
			break;
		case '3':
			// 3 to 7 seconds
			// prodocuer wait
			break;
		default:
			printf("You are not create random number!\n");
	}
	return number;
}


int main (int argc, char **argv) 
{
	mydata my_data_array[MaxThreadNum];
	int i;
	void *status;

	//TODO
	//create a random number
	// date.work_numer = ?
	// date.wait_numer = ?


	// create thread attr and mutex
	pthread_attr_t attr;
	pthread_mutex_init(&mutexnum,NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//while ( !isfull() ){
		for (i = 0; i < MaxThreadNum; i++) {
			//sleep(time);
			my_data_array[i].worknumer = 1;
			my_data_array[i].waitnumer = 2;
			pthread_create(&callThd[i], &attr, prodoucer, (void *)&my_data_array[i]);
			pthread_create(&callThd[i], &attr, consumer, (void *)&my_data_array[i]);
		}
	//}

	pthread_attr_destroy(&attr);

	/* Wait on the other threads */
	for(i=0; i<MaxThreadNum; i++)
	{
	pthread_join(callThd[i], &status);
	}

   	pthread_mutex_destroy(&mutexnum);
   	//pthread_exit(NULL);

	return 0;

}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    