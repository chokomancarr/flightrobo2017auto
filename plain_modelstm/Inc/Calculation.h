#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "mbed.h"
#include "vector.h"
class Signal
{
private:
   //vector3 vect;
   float sig_sum;
   int smp_count;
   float data_new[100];
public:
Signal();
float average_LPF(void);

float average_HPF(void);

float div(void);

void data_in(float in);

};

#endif
