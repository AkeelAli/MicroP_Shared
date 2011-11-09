/**
	Interrupt Configuration File
*/

#include "configurations.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
//NVIC includes
#include "misc.h" 
#include "LSM303DLH.h"


//Configure timer interrupt
void TIM_Configuration(){
	// TIM 2

	//Config Structs
	TIM_TimeBaseInitTypeDef timer_init;
	NVIC_InitTypeDef nvic_init;

 	//Enable clock for Timer
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//Configure Timer
	timer_init.TIM_Prescaler = 5999;  //Scale Tim clock of 72M by 6000, to get 12kHz
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;	
	timer_init.TIM_Period = 11;	//Divide 12kHz by 12 to get 1kHz timer
	timer_init.TIM_ClockDivision = 0x0; //No clock division
	TIM_DeInit(TIM2);									
	TIM_TimeBaseInit(TIM2, &timer_init);				
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		

	//Configure NVIC
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;			
	nvic_init.NVIC_IRQChannelPreemptionPriority = 3;	
	nvic_init.NVIC_IRQChannelSubPriority = 0;			
	nvic_init.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&nvic_init);							

	//Start Timer2
	TIM_Cmd(TIM2, ENABLE);								

  	TIM_ClearFlag(TIM2, TIM_FLAG_Update);			
}

//configure freefall software interrupt
void SWInt_Configuration(){
	// PORT B GPIO
	// GPIO, EXTI Pin 12
	// EXTI_15_10_IRQn

	//Config Structs
	GPIO_InitTypeDef gpioConfig;
	EXTI_InitTypeDef extiConfig;
	NVIC_InitTypeDef nvicConfig;
	
	//Enable clock for port B's GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		 //??
	
	//Configure GPIO Pin 12 on Port B to be an input (for the interrupt)
	gpioConfig.GPIO_Pin = GPIO_Pin_12;
	gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
	gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB , &gpioConfig);
	
	//Configure Pin 12 as an External interrupt source
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	
	//Configure the External Interrupt
  	extiConfig.EXTI_Line = EXTI_Line12; 
  	extiConfig.EXTI_Mode = EXTI_Mode_Interrupt;
  	extiConfig.EXTI_Trigger = EXTI_Trigger_Rising;
  	extiConfig.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiConfig);
	
	//Configure NVIC
	nvicConfig.NVIC_IRQChannel = EXTI15_10_IRQn;	
  	nvicConfig.NVIC_IRQChannelPreemptionPriority = 2;	//higher priority than TIM
  	nvicConfig.NVIC_IRQChannelSubPriority = 0; 
  	nvicConfig.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicConfig);
}

//Configure Button HW Interrupt 
void ButtonInt_Configuration(){
	// PORT C GPIO
	// GPIO, EXTI Pin 13
	// EXTI_15_10_IRQn

	//Config structs
	GPIO_InitTypeDef gpioConfig;
	EXTI_InitTypeDef extiConfig;
	NVIC_InitTypeDef nvicConfig;
	
 	//Enable clock for port C's GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//Configure GPIO Pin 13 on Port C to be an input (for the interrupt)
	gpioConfig.GPIO_Pin = GPIO_Pin_13;
	gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
	gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC , &gpioConfig);
	
	//Configure Pin 13 as an External interrupt source
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	
	//Configure the External Interrupt
  	extiConfig.EXTI_Line = EXTI_Line13; 
  	extiConfig.EXTI_Mode = EXTI_Mode_Interrupt;
  	extiConfig.EXTI_Trigger = EXTI_Trigger_Rising;
  	extiConfig.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiConfig);
	
	//Configure NVIC
	nvicConfig.NVIC_IRQChannel = EXTI15_10_IRQn;		
  	nvicConfig.NVIC_IRQChannelPreemptionPriority = 1;	//highest priority
  	nvicConfig.NVIC_IRQChannelSubPriority = 0; 
  	nvicConfig.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicConfig);


}
