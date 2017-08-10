#ifndef _PROPO_H_
#define _PROPO_H_

#include "mbed.h"

enum STICK_AUTOMODE {
	STICK_AUTO_LEFT = 1,
	STICK_AUTO_UP,
	STICK_AUTO_RIGHT,
	STICK_AUTO_DOWN
};

class _PROPO{
public:
	_PROPO(Serial*);
	static TIM_HandleTypeDef TimMasterHandle;
	static bool us_ticker_inited;
	
	static unsigned int AILE,ELEV,THRO,RUDD; //pulse time
	
	static bool is_auto;
	static unsigned int autoMode;
	
	static unsigned int start;
	static bool switch_b, switch_c, switch_f;
	static unsigned int servo_elevater,servo_rudder,servo_throttle;

	static Serial* pc;
	
	static void propo2_interrupt();
	static void propo3_interrupt();
	static void propo4_interrupt();
	static void propo7_interrupt();
	
	void UpdateServos();
};


#endif /*_PROPO_H_*/
