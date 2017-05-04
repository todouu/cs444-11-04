#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{

	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	char vendor[13];
	
	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	
	if(ecx & 0x40000000){
		//use rdrand
		unsigned int *therand;
		printf("we are using rdrand");
		unsigned char err;
 		asm volatile("rdrand %0 ; setc %1"
 			: "=r" (*therand), "=qm" (err));
 		printf("random number is %d", (int)err );


	}
	else{
		//use mt19937
		printf("we are using mt19937\n");
	}

	return 0;
}