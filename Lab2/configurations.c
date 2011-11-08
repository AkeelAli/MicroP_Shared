#include "configurations.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"

//NVIC	
#include "misc.h" 

#include "LSM303DLH.h"


  
  #define GPIO_KEY_BUTTON                   GPIOC    
  #define RCC_APB2Periph_GPIO_KEY_BUTTON    RCC_APB2Periph_GPIOC
  #define GPIO_PIN_KEY_BUTTON               GPIO_Pin_13
  #define EXTI_LINE_KEY_BUTTON              EXTI_Line13
  #define GPIO_PORT_SOURCE_KEY_BUTTON       GPIO_PortSourceGPIOC
  #define GPIO_PIN_SOURCE_KEY_BUTTON        GPIO_PinSource13


//Configure timer interrupt
void TIM_Configuration(){
 	 /* configure TIMER */	
	TIM_TimeBaseInitTypeDef timer_init;
	NVIC_InitTypeDef nvic_init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	timer_init.TIM_Prescaler = 5999;					   //??
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;	
	timer_init.TIM_Period = 11;							  //??
	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;		  //??

	TIM_DeInit(TIM2);									
	TIM_TimeBaseInit(TIM2, &timer_init);				
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		

	// Initialize NVIC settings
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;			
	nvic_init.NVIC_IRQChannelPreemptionPriority = 3;	
	nvic_init.NVIC_IRQChannelSubPriority = 0;			
	nvic_init.NVIC_IRQChannelCmd = ENABLE;			

	NVIC_Init(&nvic_init);							

	TIM_Cmd(TIM2, ENABLE);								

  	TIM_ClearFlag(TIM2, TIM_FLAG_Update);			
}

//configure freefall software interrupt
void SWInt_Configuration(){
	GPIO_InitTypeDef gpioConfig;
	EXTI_InitTypeDef extiConfig;
	NVIC_InitTypeDef nvicConfig;
	
	//ENABLE CLOCK FOR PORT B's GPIO
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
	
	//Configure the Interrupt
	nvicConfig.NVIC_IRQChannel = EXTI15_10_IRQn;		//how does EXTI15_10 correspond to line12 ??
  	nvicConfig.NVIC_IRQChannelPreemptionPriority = 2;	//higher priority than TIM
  	nvicConfig.NVIC_IRQChannelSubPriority = 0; 
  	nvicConfig.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicConfig);
}

void ButtonInt_Configuration(){
	GPIO_InitTypeDef gpioConfig;
	EXTI_InitTypeDef extiConfig;
	NVIC_InitTypeDef nvicConfig;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	gpioConfig.GPIO_Pin = GPIO_Pin_13;
	gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
	gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC , &gpioConfig);
	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	

  	extiConfig.EXTI_Line = EXTI_Line13; 
  	extiConfig.EXTI_Mode = EXTI_Mode_Interrupt;
  	extiConfig.EXTI_Trigger = EXTI_Trigger_Rising;
  	extiConfig.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiConfig);
	

	nvicConfig.NVIC_IRQChannel = EXTI15_10_IRQn;		
  	nvicConfig.NVIC_IRQChannelPreemptionPriority = 1;	//highest priority
  	nvicConfig.NVIC_IRQChannelSubPriority = 0; 
  	nvicConfig.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicConfig);


}
