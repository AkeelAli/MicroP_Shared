#include "configurations.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
//NVIC	
#include "misc.h" 

#include "LSM303DLH.h"

void TIM_Configuration(){
 	 /* configure TIMER */	
	TIM_TimeBaseInitTypeDef timer_init;
	NVIC_InitTypeDef nvic_init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	// TIM2 Clock enable

	timer_init.TIM_Prescaler = 5999;					// Set Prescaler to 60000 (72M/(59999+1) = 1200 Hz)
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;	// Set counter mode to counting upwards
	timer_init.TIM_Period = 11;							// Set clock period to 48 (1200/(47+1)) = 25 Hz)
	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;		// Set no clock division (divide by 1)

	TIM_DeInit(TIM2);									// DeInit timer 3 to clear values
	TIM_TimeBaseInit(TIM2, &timer_init);				// Configure General Purpose Timer with all selected values above
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);			// Enable TIM3 interrupts

	// Initialize NVIC settings
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;				// Set IRQChannel to use TIM3
	nvic_init.NVIC_IRQChannelPreemptionPriority = 1;	// Set Preemptive Priority to 1
	nvic_init.NVIC_IRQChannelSubPriority = 1;			// Set Subpriority to 1
	nvic_init.NVIC_IRQChannelCmd = ENABLE;				// Set Channel command to ENABLE 

	NVIC_Init(&nvic_init);								// Configure NVIC with all selected values above

	TIM_Cmd(TIM2, ENABLE);								// Enable and start Timer 3

  	TIM_ClearFlag(TIM2, TIM_FLAG_Update);				// Clear TIM3 update pending flag
}

