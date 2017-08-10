
#include "propo.h"

#define TIM_MST      TIM2
#define TIM_MST_IRQ  TIM2_IRQn
#define TIM_MST_RCC  __TIM2_CLK_ENABLE()

//自動モード切り替え用
#define STICK_CENTER_PULSEWIDTH 700//左ステックが中心にある時の周期（測って変えること）
#define STICK_CENTER_TOLERANCE 100//周期を無視する誤差幅

TIM_HandleTypeDef _PROPO::TimMasterHandle;
bool _PROPO::us_ticker_inited;
unsigned int _PROPO::AILE,_PROPO::ELEV,_PROPO::THRO,_PROPO::RUDD;
bool _PROPO::is_auto;
unsigned int _PROPO::autoMode, _PROPO::start;
bool _PROPO::switch_b, _PROPO::switch_c, _PROPO::switch_f;
unsigned int _PROPO::servo_elevater,_PROPO::servo_rudder,_PROPO::servo_throttle;
Serial* _PROPO::pc;

_PROPO::_PROPO(Serial* serial)
{
	if (us_ticker_inited) return;
    us_ticker_inited = true;
    pc = serial;

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
	if (!is_auto) servo_elevater=(ELEV>500)? 1 : 0;
	else if (autoMode == 0 && ELEV < 500) {
		if (ELEV - STICK_CENTER_PULSEWIDTH > STICK_CENTER_TOLERANCE) {
			autoMode = STICK_AUTO_UP;
			pc->printf("auto mode set to " + (autoMode));
		}
		else if (STICK_CENTER_PULSEWIDTH - ELEV > STICK_CENTER_TOLERANCE) {
			autoMode = STICK_AUTO_DOWN;
			pc->printf("auto mode set to " + (autoMode));
		}
	}
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
	if (!is_auto) servo_rudder=(RUDD>500)? 1 : 0;
	else if (autoMode == 0 && ELEV < 500) {
		if (ELEV - STICK_CENTER_PULSEWIDTH > STICK_CENTER_TOLERANCE) {
			autoMode = STICK_AUTO_RIGHT;
			pc->printf("auto mode set to " + (autoMode));
		}
		else if (STICK_CENTER_PULSEWIDTH - ELEV > STICK_CENTER_TOLERANCE) {
			autoMode = STICK_AUTO_LEFT;
			pc->printf("auto mode set to " + (autoMode));
		}
	}
}

void _PROPO::propo7_interrupt(){ //drop
	
}

