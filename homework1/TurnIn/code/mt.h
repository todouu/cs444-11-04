#pragma once
#include <stdio.h>

void init_genrand(unsigned long s);
void init_by_array(unsigned long init_key[], int key_length);
unsigned long genrand_int32(void);
long genrand_int31(void);
int genrand_real1(void);
int genrand_real2(void);
double genrand_real3(void);
double genrand_res53(void);
int mt_main(void);




