#include "stm32f4xx_hal.h"
#include "initi.h"
#include <stdlib.h>
#include "mbed.h"
#include "vector.h"
#include "rebanila.h"
//#include "hardware.h"

#include "hal/pinmap.h"
#include "eeprom.h"
#include "propo.h"

DigitalOut led1(PC_8);//LED for referee please look at regulation
DigitalOut led2(PC_9);//LED for referee please look at regulation
DigitalOut mpu9250pwr(PA_12);//MPU POWER if this pin is high mpu power on
DigitalOut SONARpwr(PA_11);//
DigitalOut servo1(PB_10);//
DigitalOut servo2(PB_5);//
DigitalOut servo3(PB_3);//
DigitalOut servo4(PC_12);//
DigitalOut servo5(PA_5);//
DigitalIn autoSw(PA_1);
DigitalOut EX1(PA_10);// DigitalOut EX2(PB_4);//
Serial twe(PB_8, PB_9);//SCL,SDA パソコンとの通信
M24C64 eeprom;//eeprom
I2C i2c(D14,D15);//気圧センサの読み取り用。気圧を用いて高度を計る　
char datawrite[2];//気圧センサ送信データ格納
char data[1];//気圧センサデータ受信用
char data3h[3];//気圧センサデータ格納用。気圧センサのデータは24bitで
//３つにわけておくられてくる。
int32_t datah;// 気圧センサのデータ結合
float datahpa;　//気圧(hpa)
void PinInit();
void NVIC_Init();
int main(void)
{
	PinInit();
	HAL_Init();
	MX_GPIO_Init();
	NVIC_Init();
	Rebanila rebanila = Rebanila();
	while (1)
	{
		twe.printf("");
  		datawrite[0]=0x28;
	  i2c.write(0b1011100<<1, datawrite, 1, 1);//気圧データ受信 
	  i2c.read(0b1011100<<1, data, 1, 0);
	  data3h[0] =data[0];
	  datawrite[0]=0x28;
	  i2c.write(0b1011100<<1, datawrite, 1, 1); 
	  i2c.read(0b1011100<<1, data, 1, 0);
	  data3h[1] =data[0];
	  datawrite[0]=0x28;
	  i2c.write(0b1011100<<1, datawrite, 1, 1); 
	  i2c.read(0b1011100<<1, data, 1, 0);
	  data3h[2] =data[0];
      datah=data3h[0] | data3h[1]<<8 | data3h[2]<<16;
      datahpa=datah/4096;//気圧算出
	}
}

void PinInit(){
	pin_function(PB_6, STM_PIN_DATA(STM_MODE_IT_FALLING, PullNone, 0));//DRDY for LPS22HB
	pin_function(PA_0, STM_PIN_DATA(STM_MODE_IT_RISING_FALLING, PullUp, 0));//PROPO1　this used for AILERON (not used (yet))
	pin_function(PB_1, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//2 this is used for elevater
	pin_function(PB_2, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//3 this used for  throttle
	pin_function(PB_0, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//4 this is used for rudder
	pin_function(PA_1, STM_PIN_DATA(STM_MODE_IT_FALLING, PullUp, 0));//5 auto mode on/off
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
