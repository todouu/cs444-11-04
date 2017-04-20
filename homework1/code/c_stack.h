#include <stdio.h>
#include "concurrency1.h"

//stack varb
int MAXSIZE = 32;       
mydata *stack[32];     
int top = -1; 

int isempty();
int isfull();
mydata peek();
mydata* pop();
int push(mydata data);