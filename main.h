/**
 * @file main.c
 * @author Nguyen Khac Duy (duy.nguyen.2001_bk@hcmut.edu.vn)
 * @brief Control position of DC motor using STM32F407 DISCOVERY and display data on computer
 * @version 0.1
 * @date 2022-12-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx.h"
#include "system_timetick.h"
#include <stdbool.h>
#include <stdio.h>

// Button pin def
#define BUILTIN_BUTTON_PORT GPIOA
#define BUILTIN_BUTTON_PIN GPIO_Pin_0
#define ENC_BUTTON_PORT
#define ENC_BUTTON_PIN
// LEDs pin def
#define BUILTIN_LED_PORT GPIOA
#define BUILTIN_LED_0 GPIO_Pin_12
#define BUILTIN_LED_1 GPIO_Pin_13
#define BUILTIN_LED_2 GPIO_Pin_14
#define BUILTIN_LED_3 GPIO_Pin_15
// Potentiometer (ADC) pin def
#define POT_PORT GPIOC
#define POT_PIN GPIO_Pin_1

	void RCC_setup(void);
	void GPIO_setup(void);
	void ADC1_setup(void);
	void TIM_setup(void);
	void USART3_setup(void);
	uint16_t ADC_read(ADC_TypeDef *ADCx, uint8_t channel);
	void USART_sendStr(USART_TypeDef *USARTx, char *str);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
