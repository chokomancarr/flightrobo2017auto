#include "rebanila.h"


DigitalOut servo11(PB_10);
DigitalOut servo22(PB_5);
DigitalOut servo44(PC_12);
DigitalIn Sw1(PA_1);
DigitalIn Sw2(PA_2);
Rebanila::Rebanila(Serial* serial) : _PROPO(serial)
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

void Rebanila::out_interrupt()//こいつ自体は1μ秒ごとに呼び出される。
{
unsigned int32_t count;
unsigned int SWB_start;
unsigned int SWC_buf;
unsigned int SWB_start;
unsigned int SWC_buf;
insighed bool PA_1_before;
insighed bool PA_2_before;

staic int SWB
	//どこに置くか知らないからとりあえずここ
　　　　　
	//SwBとSwCの入力を取る 0.1ms(100μ秒ごとに)ごとに呼び出したい。
        if(count%100==0)
        {
		count++;
		if(PA_1_before==0&&PA_1==1)SWB_start=count;
		if(PA_1_before==1&&PA_1==0)SWB_buf=count-SWB_start;//
		if(PA_2_before==0&&PA_2==1)SWC_start=count;
		if(PA_2_before==1&&PA_2==0)SWC_buf=count-SWC_start;　　
		if(SWB_buf>160)switch_b==true;
                else if(SWB_buf<140)switch_b==false;
		else if(SWC_buf>160)switch_c==true;
                if(SWC_buf<140)switch_c==false;
		PA_1_before=PA_1;
		PA_2_before=PA_2;
	}

	bool i = is_auto;
	is_auto = switch_b;
	if (is_auto != i) autoMode = 0;

	servo11=servo_rudder;
	servo22=servo_elevater;
	servo44=servo_throttle;
}
