#include "stm32f4xx_hal.h"
#include "main.h"
#ifndef __INITI_H
#define __INITI_H


#ifdef __cplusplus
 extern "C" {
#endif

void SystemClock_Config(void);
void Error_Handler(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Encoder_MspInit_MX(TIM_HandleTypeDef* htim_encoder);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm);

#ifdef __cplusplus
}
#endif

#endif
