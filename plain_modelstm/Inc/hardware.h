//this file set the pin mode
/*
#ifndef _HARDWARE_H_
#define _HARDWARE_H_


#include "hal/pinmap.h"
#include "eeprom.h"
#include "propo.h"
 DigitalOut led1(PC_8);//LED for refelly please look at reguration
 DigitalOut led2(PC_9);//LED for refelly please look at reguration
 DigitalOut mpu9250pwr(PA_12);//MPU POWER if this pin is high mpu power on
 DigitalOut SONARpwr(PA_11);//

 DigitalOut servo1(PB_10);//
 DigitalOut servo2(PB_5);//
 DigitalOut servo3(PB_3);//
 DigitalOut servo4(PC_12);//
 DigitalOut servo5(PA_5);//
 DigitalOut EX1(PA_10);//
 DigitalOut EX2(PB_4);//


 Serial twe(PB_8, PB_9);//SCL,SDA

 M24C64 eeprom;//eeprom


 void PinInit();



void NVIC_Init();

#endif /*_HARDWARE_H_*/
