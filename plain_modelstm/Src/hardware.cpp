#include "hardware.h"
/*

void PinInit(){
	pin_function(PB_6, STM_PIN_DATA(STM_MODE_IT_FALLING, PullNone, 0));//DRDY for LPS22HB
	pin_function(PA_0, STM_PIN_DATA(STM_MODE_IT_RISING_FALLING, PullUp, 0));//PROPO1Å@this used for AILERON
	pin_function(PB_1, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//2 this is used for elevater
	pin_function(PB_2, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//3 this used for  throttle
	pin_function(PB_0, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//4 this is used for rudder
	pin_function(PA_1, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//5 not used
	pin_function(PA_2, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//6 not used
	pin_function(PA_3, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//7
				}




void NVIC_Init(){
  //  	NVIC_SetVector(EXTI9_5_IRQn, (uint32_t) _PROPO::propo0_interrupt);
		NVIC_SetVector(EXTI1_IRQn, (uint32_t) _PROPO::propo2_interrupt);
		NVIC_SetVector(EXTI2_IRQn, (uint32_t) _PROPO::propo3_interrupt);
		NVIC_SetVector(EXTI0_IRQn, (uint32_t) _PROPO::propo4_interrupt);
		NVIC_SetVector(EXTI3_IRQn, (uint32_t) _PROPO::propo7_interrupt);
	//	NVIC_SetVector(RTC_Alarm_IRQn,(uint32_t)AttitudeUpdate);//EXTI17 Attitude update

//		NVIC_SetPriority(EXTI9_5_IRQn,1);//ticker
//		NVIC_SetPriority(EXTI15_10_IRQn,1);
		NVIC_SetPriority(EXTI1_IRQn,1);
		NVIC_SetPriority(EXTI2_IRQn,1);
		NVIC_SetPriority(EXTI0_IRQn,1);
		NVIC_SetPriority(EXTI3_IRQn,1);
		NVIC_SetPriority(TIM4_IRQn,0);//Servo
	//	NVIC_SetPriority(TIM5_IRQn,4);//Ticker
	//	NVIC_SetPriority(RTC_Alarm_IRQn,3);//EXTI17 Attitude update
//		NVIC_SetPriority(USART6_IRQn,2);
		NVIC_SetPriority(SysTick_IRQn,0);

//		NVIC_EnableIRQ(EXTI9_5_IRQn);
		NVIC_EnableIRQ(EXTI1_IRQn);
		NVIC_EnableIRQ(EXTI2_IRQn);
		NVIC_EnableIRQ(EXTI0_IRQn);
		NVIC_EnableIRQ(EXTI3_IRQn);
}
*/
