/* 
 * Programmers: Andrew Bowers, Shuai Peng, Anya Lehman 
 * Class: CS 444 - Operating Systems II
 * Date: 4/19/17
 * Description: The producer-consumer problem
 */

#include "concurrency1.h"
#include "mt.c"

#define MaxThreadNum 5
#define MaxJob 32

//phtread varb
pthread_mutex_t mutexnum;
pthread_cond_t c_empty;
pthread_cond_t c_full;

int work[MaxJob];
int work_count;

int get_random_32(unsigned long long *arand) {
        int i ;
        for ( i = 0; i < RETRY_LIMIT; i ++) {
                 if(_rdrand32_step(arand) ) {
                 return (*arand);
						                }
			        }
	        return RETRY_LIMIT_EXCEEDED;
}

void *consumer(void* data)
{
	printf("I am conuser! \n");
	//create local data
	mydata *local = (mydata*) data;
	//local.sleeptime = data->waitnumer;


	//if is empty, just wait
	while(work_count == 0){
		pthread_cond_wait(&c_empty, &mutexnum);
	}

	//protect data
	pthread_mutex_lock (&mutexnum);

	//remove
	//work[work_count] = 0;
	work_count--;
	printf("Consumer delete number %d \n",local->worknumer);

	//unlock the data
	pthread_mutex_unlock (&mutexnum);

	//let other thread know, u can work!
	if(work_count == MaxJob -1){
		pthread_cond_broadcast(&c_full);
	}

	//sleep
	sleep(local->waitnumer);
	int take_out_numer = work[work_count + 1];
	printf("Consumer take numer is %d \n", take_out_numer);

	printf("\n");
}

void *prodoucer(void* data)
{
	printf("I am producer! \n");
	//create local data
	mydata *local = (mydata *) data;

	//if work is full, just wait
	while(work_count == MaxJob){
		pthread_cond_wait(&c_full, &mutexnum);
	}
	//puts("out of while");

	//lock the data
	pthread_mutex_lock (&mutexnum);

	//puts("I am locking");

	//do work
	work[work_count] = local->worknumer;
	work_count++;
	printf("Prodoucer add number %d \n",local->worknumer);

	//unlock the data
	pthread_mutex_unlock (&mutexnum);
	//puts("I am unlocking");
	
	//let other thread know, u can work!
	if(work_count){
	//	puts("I am let other know u can work");
		pthread_cond_broadcast(&c_empty);
	}

	//puts("go sleep!");

	sleep(local->waitnumer);
	printf("\n");
}

int my_rand(int cases)
{
	int number;
	switch (cases){
		case 1:
			//any number we want
			//store data
			if (check_method() == 0){
				//do rdrand
			}
			else{
				number = (int) genrand_int32();
			}
			break;
		case 2:
			// 2 to 9 seconds
			// consumer wait
			if (check_method() == 0){
				//do rdrand
			}
			else{
				number = (int) genrand_real2();
			}
			break;
		case 3:
			// 3 to 7 seconds
			// prodocuer wait
			if (check_method() == 0){
				//do rdrand
			}
			else{
				number = (int) genrand_real1();
			}
			break;
		default:
			printf("You are not create random number!\n");
	}
	//printf("The number you have is %d \n" , number);
	return number;
}

int check_method()
{
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	//char vendor[13];
	
	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );

	if(ecx & 0x40000000){
		//use rdrand
		return 0;
	}
	else{
		//use mt19937
		return 1;
	}
}


int main (int argc, char **argv) 
{
	//array of data we need pass
	mydata my_data_array[MaxThreadNum];
	//create thread
	pthread_t pro_thread[MaxThreadNum];
	pthread_t con_thread[MaxThreadNum];

	int i;
	//void *status;

	// create thread attr and mutex
	//pthread_attr_t attr;
	pthread_mutex_init(&mutexnum,NULL);
	//pthread_attr_init(&attr);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_cond_init(&c_full, NULL);
	pthread_cond_init(&c_empty, NULL);
	work_count = 0;

	while ( work_count != MaxJob -1 ){
		for (i=0; i < 3 ; i++){
			puts("I am create prodoucer");
			my_data_array[i].worknumer = my_rand(1);
			my_data_array[i].waitnumer = my_rand(3);
			pthread_create(&pro_thread[i], NULL, prodoucer, (void *)&my_data_array[i]);

		}

		for (i = 0; i < 2; i++) {
			//create random number
			puts("I am create consumer");
			my_data_array[i].worknumer = my_rand(1);
			my_data_array[i].waitnumer = my_rand(2);
			pthread_create(&con_thread[i], NULL, consumer, (void *)&my_data_array[i]);
		}
		printf("Total count is %d \n", work_count);
	}

	//pthread_attr_destroy(&attr);

	/* Wait on the other threads */
	//for(i=0; i<MaxThreadNum; i++)
	//{
	//	pthread_join(pro_thread[i], &status);
	//	pthread_join(con_thread[i], &status);
	//}

   	pthread_mutex_destroy(&mutexnum);

	return 0;
}
