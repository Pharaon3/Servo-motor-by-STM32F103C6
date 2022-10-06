#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "delay.h"

volatile int count;
volatile int bigCount;
volatile int Mode;		//   1: up, 0: down
volatile int NN;			//	 it's for bigCount change  1: twice, 2: thrice...
volatile int nCount; //   it's for counting to NN

void TIM2_INT_Init(void);

void TIM2_IRQHandler()
{
	// Checks whether the TIM2 interrupt has occurred or not
	if (TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		count ++;
		if( count < bigCount ) {
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
			if ( Mode == 1 ) GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//It's for LED blinking.  You can remove it.
			else GPIO_SetBits(GPIOA, GPIO_Pin_4);								//It's for LED blinking.  You can remove it.
		}
		else {
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);
			if ( Mode == 1 ) GPIO_SetBits(GPIOA, GPIO_Pin_4);		//It's for LED blinking.  You can remove it.
			else GPIO_ResetBits(GPIOA, GPIO_Pin_4);							//It's for LED blinking.  You can remove it.
		}
		if (count >= 2000) {
			count = 0;
			if ( Mode == 1) {		// up			The servo motor is rotating from 0 degree to 180 degree.
				nCount ++;
				if (nCount > NN) {
					nCount =0;
					bigCount ++;
					if (bigCount >= 250){		// max degree 180
						bigCount = 250;
						Mode = 0;
					}
				}
			}
			else {		// down							The servo motor is rotating from 180 degree to 0 degree.
				nCount ++;
				if (nCount > NN) {
					nCount =0;
					bigCount --;
					if (bigCount <= 50) {		// min degree 0
						bigCount =50;
						Mode = 1;
					}
				}
			}
		}
		// Clears the TIM2 interrupt pending bit
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

GPIO_InitTypeDef GPIO_InitStruct;

int main(void)
{
	count = 0;
	bigCount = 50;		// Initial state.  0 degree
	Mode = 1;		//   1: up, 0: down
	NN = 1;			//	 it's for bigCount change  1: twice, 2: thrice       This parameter is set for speed. increase for speen down, decrease for speed up.  It must be integer.
	nCount = 0; //   it's for counting to NN
	DelayInit();
	
	// Initialize timer interrupt
	TIM2_INT_Init();
	
	// Initialize PB6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	
	// Initialize PA4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	while (1)
	{
	}
}

void TIM2_INT_Init()
{
	// Init struct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// TIM2 initialization for overflow every 500ms
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event (Hz) = 72MHz / ((99 + 1) * (71 + 1)) = 100KHz 
	TIM_TimeBaseInitStruct.TIM_Prescaler = 9;
	TIM_TimeBaseInitStruct.TIM_Period = 71;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// Enable TIM2 interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// Start TIM2
	TIM_Cmd(TIM2, ENABLE);
	
	// Nested vectored interrupt settings
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

