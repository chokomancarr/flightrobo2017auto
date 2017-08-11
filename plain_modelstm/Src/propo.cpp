
#include "propo.h"

#define TIM_MST      TIM2
#define TIM_MST_IRQ  TIM2_IRQn
#define TIM_MST_RCC  __TIM2_CLK_ENABLE()

//自動モード切り替え用
#define STICK_CENTER_PULSEWIDTH 700//右ステックが中心にある時の周期（測って変えること）
#define STICK_CENTER_TOLERANCE 100//周期を無視する誤差幅

TIM_HandleTypeDef _PROPO::TimMasterHandle;
bool _PROPO::us_ticker_inited;
unsigned int _PROPO::AILE,_PROPO::ELEV,_PROPO::THRO,_PROPO::RUDD;
unsigned int _PROPO::AILE_ST,_PROPO::ELEV_ST,_PROPO::THRO_ST,_PROPO::RUDD_ST;
bool _PROPO::is_auto;
unsigned int _PROPO::autoMode;
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

void _PROPO::propo1_interrupt(){ //aileron (setting auto mode)
	if (!is_auto || autoMode > 0) return;
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<2) != RESET) {//PROPO1
		__HAL_GPIO_EXTI_CLEAR_IT(1<<2);
		buf=TIM2->CNT;
		if(buf>AILE_ST)AILE=buf-AILE_ST;
		else AILE=0xFFFFFFFFUL-AILE_ST+buf;
		AILE_ST=buf;
	}
	if (AILE - STICK_CENTER_PULSEWIDTH > STICK_CENTER_TOLERANCE) autoMode = switch_c? 3 : 1;
	else if (STICK_CENTER_PULSEWIDTH - AILE > STICK_CENTER_TOLERANCE) autoMode = switch_c? 4 : 2;
#ifdef _DebugMode
	pc->printf("auto mode set to " + autoMode);
#endif
}

void _PROPO::propo2_interrupt(){ //elevator (up down)
	if (is_auto) return;
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<2) != RESET) {//PROPO2
		__HAL_GPIO_EXTI_CLEAR_IT(1<<2);
		buf=TIM2->CNT;
		if(buf>ELEV_ST)ELEV=buf-ELEV_ST;
		else ELEV=0xFFFFFFFFUL-ELEV_ST+buf;
		ELEV_ST=buf;
	}
	servo_elevater=(ELEV>500)? 1 : 0;
}

void _PROPO::propo3_interrupt(){ //throttle (motor)
	if (is_auto) return;
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<1) != RESET) {//PROPO3
		__HAL_GPIO_EXTI_CLEAR_IT(1<<1);
		buf=TIM2->CNT;
		if(buf>THRO_ST)THRO=buf-THRO_ST;
		else THRO=0xFFFFFFFFUL-THRO_ST+buf;
		THRO_ST=buf;
	}
	servo_throttle=(THRO>500)? 1 : 0;
}
void _PROPO::propo4_interrupt(){ //rudder (left right)
	if (is_auto) return;
	unsigned int buf;
	if (__HAL_GPIO_EXTI_GET_IT(1<<0) != RESET) {//PROPO4
		__HAL_GPIO_EXTI_CLEAR_IT(1<<0);
		buf=TIM2->CNT;
		if(buf>RUDD_ST)RUDD=buf-RUDD_ST;
		else RUDD=0xFFFFFFFFUL-RUDD_ST+buf;
		RUDD_ST=buf;
	}
	servo_rudder=(RUDD>500)? 1 : 0;
}

void _PROPO::propo7_interrupt(){ //drop
	
}

