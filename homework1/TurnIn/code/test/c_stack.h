#ifndef c_stack
#define c_stack

#include <stdio.h>
#include "concurrency1.h"

int isempty();
int isfull();
mydata peek();
mydata* pop();
int push(mydata *data);

#endif
