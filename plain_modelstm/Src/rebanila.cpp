#include "rebanila.h"

DigitalOut servo11(PB_10);
DigitalOut servo22(PB_5);
DigitalOut servo44(PC_12);
DigitalIn Sw1(PA_1);
DigitalIn Sw2(PA_2);

unsigned long Rebanila::counter = 0;

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

unsigned int SWB_start;
unsigned int SWB_buf;
unsigned int SWC_start;
unsigned int SWC_buf;
bool Sw1_old;
bool Sw2_old;

#ifdef _DebugMode
bool switch_b_old, switch_c_old;
#endif

void Rebanila::out_interrupt() {//‚±‚¢‚ÂŽ©‘Ì‚Í1ƒÊ•b‚²‚Æ‚ÉŒÄ‚Ño‚³‚ê‚éB
	//SwB‚ÆSwC‚Ì“ü—Í‚ðŽæ‚é 0.1ms(100ƒÊ•b‚²‚Æ‚É)‚²‚Æ‚ÉŒÄ‚Ño‚µ‚½‚¢B
	counter++;
	if(counter%100 == 0)
	{
		if(!Sw1_old && Sw1) SWB_start=counter;
		if(Sw1_old && !Sw1) SWB_buf=counter-SWB_start;//
		if(!Sw2_old && Sw2) SWC_start=counter;
		if(Sw2_old && !Sw2) SWC_buf=counter-SWC_start;

		if(SWB_buf>160) switch_b=true;
		else if(SWB_buf<140) switch_b=false;
		if(SWC_buf>160) switch_c=true;
		else if(SWC_buf<140) switch_c=false;

#ifdef _DebugMode
		if (switch_b != switch_b_old) {
			switch_b_old = switch_b;
			pc->printf("switch b is now " + (switch_b? "1" : "0"));
		}
		if (switch_c != switch_c_old) {
			switch_c_old = switch_c;
			pc->printf("switch c is now " + (switch_c? "1" : "0"));
		}
#endif

		Sw1_old=Sw1;
		Sw2_old=Sw2;
	}

	bool i = is_auto;
	is_auto = switch_b;
	if (is_auto != i) autoMode = 0;

	servo11=servo_rudder;
	servo22=servo_elevater;
	servo44=servo_throttle;
}
