#include "c_stack.h"

//stack varb
int MAXSIZE = 32;       



int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

mydata peek() {
   return *stack[top];
}
mydata *pop() {
   mydata *data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");

   }
}

int push(mydata *data, mydata* stack) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
      return 0;
   } else {
      printf("Could not insert data, Stack is full.\n");
      return 1;
   }
}
