#ifndef __REBANILA_H
#define __REBANILA_H

#include "mbed.h"
#include "propo.h"

class Rebanila : public _PROPO
{
public:
	Rebanila();
	
	static void out_interrupt();
};
#endif
