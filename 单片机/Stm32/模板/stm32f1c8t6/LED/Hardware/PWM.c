#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	
	//配置内部时钟源
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置定时器
	TIM_InternalClockConfig(TIM2);
	
	//时基单元结构体定义
	TIM_TimeBaseInitTypeDef TIM_TimeBaesInitStructure;
	TIM_TimeBaesInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaesInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaesInitStructure.TIM_Period = 1000-1;	//AAR
	TIM_TimeBaesInitStructure.TIM_Prescaler = 7200-1;	//PSC
	TIM_TimeBaesInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaesInitStructure);
	
	//输出比较初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;	//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	//使能计数器
	TIM_Cmd(TIM2,ENABLE);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
	
}

void PWM_SetPrescaler(uint16_t Prescale)
{
	TIM_PrescalerConfig(TIM2,Prescale,TIM_PSCReloadMode_Immediate);
}


