
#include "propo.h"


#define TIM_MST      TIM2
#define TIM_MST_IRQ  TIM2_IRQn
#define TIM_MST_RCC  __TIM2_CLK_ENABLE()

TIM_HandleTypeDef _PROPO::TimMasterHandle;
bool _PROPO::us_ticker_inited;
unsigned int _PROPO::AILE,_PROPO::ELEV,_PROPO::THRO,_PROPO::RUDD;
bool _PROPO::is_auto;
unsigned int _PROPO::autoMode, _PROPO::start;
unsigned int _PROPO::servo_elevater,_PROPO::servo_rudder,_PROPO::servo_throttle;

_PROPO::_PROPO()
{
	if (us_ticker_inited) return;
    us_ticker_inited = true;

    // Enable timer clock
    TIM_MST_RCC;

    // Configure time base
    TimMasterHandle.Instance = TIM_MST;
    TimMasterHandle.Init.Period            = 0xFFFFFFFF;
    TimMasterHandle.Init.Prescaler         = (uint32_t)(SystemCoreClock / 1000000) - 1; // 1 us tick
    TimMasterHandle.Init.ClockDivision     = 0;
    TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimMasterHandle.Init.RepetitionCounter = 0;
    HAL_TIM_OC_Init(&TimMasterHandle);

//    NVIC_SetVector(TIM_MST_IRQ, (uint32_t)us_ticker_irq_handler);
//    NVIC_EnableIRQ(TIM_MST_IRQ);

    // Enable timer
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);
}

void _PROPO::propo2_interrupt(){ //elevator (up down)
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<2) != RESET)
	{//PROPO2
		__HAL_GPIO_EXTI_CLEAR_IT(1<<2);
		buf=TIM2->CNT;
		if(buf>start)ELEV=buf-start;
		else ELEV=0xFFFFFFFFUL-start+buf;
		start=buf;
	}
	servo_elevater=(ELEV>500)? 1 : 0;
}

void _PROPO::propo3_interrupt(){ //throttle (motor)
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<1) != RESET) {//PROPO3
		__HAL_GPIO_EXTI_CLEAR_IT(1<<1);
		buf=TIM2->CNT;
		if(buf>start)THRO=buf-start;
		else THRO=0xFFFFFFFFUL-start+buf;
		start=buf;
	}
	servo_throttle=(THRO>500)? 1 : 0;
}
void _PROPO::propo4_interrupt(){ //rudder (left right)
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<0) != RESET) {//PROPO4
		__HAL_GPIO_EXTI_CLEAR_IT(1<<0);
		buf=TIM2->CNT;
//		if(buf>start)AILE=buf-start;
//		else AILE=0xFFFFFFFFUL-start+buf;
		if(buf>start)RUDD=buf-start;
		else RUDD=0xFFFFFFFFUL-start+buf;
		start=buf;
	}
	servo_rudder=(RUDD>500)? 1 : 0;
}

void _PROPO::propo7_interrupt(){
	
}

