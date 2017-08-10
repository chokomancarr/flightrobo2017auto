#include "rebanila.h"


static TIM_HandleTypeDef TimMasterHandle;
DigitalOut servo11(PB_10);
DigitalOut servo22(PB_5);
DigitalOut servo44(PC_12);

Rebanila::Rebanila()
{
	__TIM4_CLK_ENABLE();

	// Configure time base
	TimMasterHandle.Instance = TIM4;
	TimMasterHandle.Init.Period = 0xFFFF;
	TimMasterHandle.Init.Prescaler = (uint32_t) (SystemCoreClock / 1000000) - 1; // 1us tick
	TimMasterHandle.Init.ClockDivision = 0;
	TimMasterHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimMasterHandle.Init.RepetitionCounter = 0;
	HAL_TIM_OC_Init(&TimMasterHandle);
	NVIC_SetVector(TIM4_IRQn, (uint32_t) out_interrupt);
	NVIC_EnableIRQ(TIM4_IRQn);
	// Enable timer
	HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);
	// Set new output compare value
	__HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, 0);
	// Enable IT
	__HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void Rebanila::out_interrupt()
{
	servo11=servo_rudder;
	servo22=servo_elevater;
	servo44=servo_throttle;
}
