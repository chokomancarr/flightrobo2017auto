#include "Calculation.h"

Signal::Signal()
{
	sig_sum=0;
	smp_count=0;
}

float Signal::average_LPF()
{
	return (sig_sum/smp_count);
}

float Signal::average_HPF()
{
	float temp;
	temp=average_LPF();
	return (data_new[0]-temp);

}

float Signal::div(void)
{
	return (data_new[0]-data_new[1]);
}

void Signal::data_in(float in)
{
	for(int i=99;i>0;i--)
	{
		data_new[i]=data_new[i-1];
	}
	data_new[0]=in;
	sig_sum+=in;
	smp_count++;
	return;
}
