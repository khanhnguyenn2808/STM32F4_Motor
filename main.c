/**
 * @file main.c
 * @author Nguyen Khac Duy (duy.nguyen.2001_bk@hcmut.edu.vn)
 * @brief Control position of DC motor using STM32F407 DISCOVERY and display data on computer
 * @version 0.1
 * @date 2022-12-12
 *
 * @copyright Copyright (c) 2022
 * main
 */
#include <main.h>

#define Tx_BUFF_SIZE 16
#define Rx_BUFF_SIZE 1
#define CMD_BUFF_SIZE 4
#define TIM_Freq 1000000.0f
#define PWM_Freq 10000.0f
#define ERROR 20
#define SCALE 1

float PWM_Duty = 100;
char Tx_buff[Tx_BUFF_SIZE] = {' '};
char Rx_buff[Rx_BUFF_SIZE];
char CMD_index = 0;
char CMD_buff[CMD_BUFF_SIZE];
bool Rx;
bool done;
bool request = false;
bool useENC = true;

float setPoint;
float curVal;
float error;

int main()
{
	//* [SETUP]
	SystemInit();
	SysTick_Config(SystemCoreClock / 1000); // 1ms interupt
	RCC_setup();
	GPIO_setup();
	USART3_setup();
	TIM_setup();
	USART_sendStr(USART3, "<Setup done!>");

	GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET);
	GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
	Tx_buff[Tx_BUFF_SIZE - 1] = '\0';
	CMD_buff[CMD_BUFF_SIZE - 1] = '\0';

	bool button_state;

	//* [LOOP]
	while (1)
	{
		if (!(tick_count % 50))
		{
			button_state = GPIO_ReadInputDataBit(BUILTIN_BUTTON_PORT, BUILTIN_BUTTON_PIN);
			if (useENC)
				setPoint = TIM3->CNT * 360. / 39.;
			else if (done)
			{
				setPoint = atoi(CMD_buff);
				done = false;
			}

			curVal = TIM4->CNT * 360. / 503.;

			if (curVal < setPoint)
			{
				error = setPoint - curVal;
				if (error < 360. / 2)
				{
					GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET); // cw
					GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
				}
				else
				{
					GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET);
					GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET); // ccw
				}
			}
			else if (setPoint < curVal)
			{
				error = curVal - setPoint;
				if (error < 360. / 2)
				{
					GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET);
					GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET);
				}
				else
				{
					GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET);
					GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
				}
			}
			else
			{
				GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET);
				GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
			}

			// PWM_Duty = (error /2.) * (100. / 180.);
			PWM_Duty = error > 5. ? 60. : 0.;
			TIM5->CCR2 = ((PWM_Freq / TIM_Freq) * 100. * PWM_Duty) - 1;

			if (button_state)
			{
				TIM_SetCounter(TIM3, 0);
				TIM_SetCounter(TIM4, 0);
				setPoint = 0;
				curVal = 0;
			}
		}
		if (tick_count == 200)
		{
			sprintf(Tx_buff, "%6.2lf-%6.2lf-%i\n",
					setPoint,
					curVal,
					useENC);
			if (request)
			{
				DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
				DMA1_Stream3->NDTR = Tx_BUFF_SIZE;
				DMA_Cmd(DMA1_Stream3, ENABLE);
				request = false;
			}
			tick_count = 0;
		}
	}
}

/**
 * @brief Enable peripherals clock
 * [GPIOA,B,C,D; TIM3,4,5; USART3; DMA1; ADC1]
 */
void RCC_setup()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_DMA1EN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM5EN | RCC_APB1ENR_USART3EN, ENABLE);
}

/**
 * @brief Setup GPIO Pins
 * [LEDs, BUTTON, USART, ADC, PWM, ENC, ENC_MOTOR]
 */
void GPIO_setup()
{
	GPIO_InitTypeDef gpioInit;
	// Builtin BUTTON | PA0
	gpioInit.GPIO_Pin = GPIO_Pin_0;
	gpioInit.GPIO_Mode = GPIO_Mode_IN;
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpioInit);

	// MOTOR_DIR & Builtin LEDs | PD14,15
	gpioInit.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	gpioInit.GPIO_Mode = GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &gpioInit);

	// USART3 | PC10:Tx PC11:Rx
	gpioInit.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInit);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); // Tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3); // Rx

	// PWM (TIM5 AF2) | PA1: CH2
	gpioInit.GPIO_Pin = GPIO_Pin_1;
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInit.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &gpioInit);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

	// ENC_MOTOR (TIM4 AF2) | PB6,7
	gpioInit.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpioInit);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

	// ENC (TIM3 AF2) | PB4:A PB5:B
	gpioInit.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInit.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpioInit);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

	// ENC_BUTTON | PB2
	gpioInit.GPIO_Pin = GPIO_Pin_2;
	gpioInit.GPIO_Mode = GPIO_Mode_IN;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpioInit);
}

/**
 * @brief Setup Timer
 * [TIM3:ENC_MOTOR; TIM4:ENC]
 */
void TIM_setup()
{
	// setup encoder
	TIM_EncoderInterfaceConfig(TIM3,
							   TIM_EncoderMode_TI1,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM3, 39);
	TIM_Cmd(TIM3, ENABLE);
	TIM_SetCounter(TIM3, 0);

	TIM_EncoderInterfaceConfig(TIM4,
							   TIM_EncoderMode_TI1,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM4, 252 * 2 - 1);
	TIM_Cmd(TIM4, ENABLE);
	TIM_SetCounter(TIM4, 0);

	// setup PWM output
	TIM_TimeBaseInitTypeDef timInit;
	timInit.TIM_CounterMode = TIM_CounterMode_Up;
	timInit.TIM_Period = 100;
	timInit.TIM_ClockDivision = 0;
	timInit.TIM_Prescaler = (SystemCoreClock / 2) / PWM_Freq - 1;
	timInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &timInit);

	TIM_OCInitTypeDef timoc;
	timoc.TIM_OCMode = TIM_OCMode_PWM1;
	timoc.TIM_OutputState = TIM_OutputState_Enable;
	timoc.TIM_Pulse = 0;
	timoc.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM5, &timoc);
	TIM_Cmd(TIM5, ENABLE);
}

void USART3_setup(void)
{
	USART_InitTypeDef usartInit;
	USART_StructInit(&usartInit);
	usartInit.USART_BaudRate = 9600;
	USART_Init(USART3, &usartInit);
	USART_Cmd(USART3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	DMA_InitTypeDef dmaInit;
	dmaInit.DMA_Channel = DMA_Channel_4;
	dmaInit.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
	dmaInit.DMA_Memory0BaseAddr = (uint32_t)Tx_buff;
	dmaInit.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dmaInit.DMA_BufferSize = Tx_BUFF_SIZE;
	dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dmaInit.DMA_Mode = DMA_Mode_Normal;
	dmaInit.DMA_Priority = DMA_Priority_High;
	dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream3, &dmaInit);
	DMA_Cmd(DMA1_Stream3, ENABLE);

	dmaInit.DMA_Channel = DMA_Channel_4;
	dmaInit.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
	dmaInit.DMA_Memory0BaseAddr = (uint32_t)Rx_buff;
	dmaInit.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dmaInit.DMA_BufferSize = Rx_BUFF_SIZE;
	dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dmaInit.DMA_Mode = DMA_Mode_Normal; // DMA_Mode_Circular;
	dmaInit.DMA_Priority = DMA_Priority_High;
	dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &dmaInit);
	DMA_Cmd(DMA1_Stream1, ENABLE);

	NVIC_InitTypeDef NVICInit;
	NVICInit.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInit.NVIC_IRQChannelSubPriority = 0;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICInit);
	DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
}

/**
 * @brief Send a string to USART
 *
 * @param USARTx 1 - 6
 * @param str string to send
 */
void USART_sendStr(USART_TypeDef *USARTx, char *str)
{
	while (*str != '\0')
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USARTx, *(str++));
	}
}

void DMA1_Stream1_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);
	if (Rx_buff[0] == 'c')
		useENC = !useENC;
	else if (Rx_buff[0] == 'r')
		request = true;
	else
		CMD_buff[CMD_index++] = Rx_buff[0];

	if (CMD_index >= CMD_BUFF_SIZE - 1)
	{
		CMD_index = 0;
		done = true;
	}
	DMA_Cmd(DMA1_Stream1, ENABLE);
}
