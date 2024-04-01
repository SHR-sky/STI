#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	//配置内部时钟源
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置定时器
	TIM_InternalClockConfig(TIM2);
	
	//时基单元结构体定义
	TIM_TimeBaseInitTypeDef TIM_TimeBaesInitStructure;
	TIM_TimeBaesInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaesInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaesInitStructure.TIM_Period = 0xFFFF-1;	//AAR
	TIM_TimeBaesInitStructure.TIM_Prescaler = 72-1;	//PSC
	TIM_TimeBaesInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaesInitStructure);
	
	//输入捕获的设置——频率测量
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//输入捕获的设置——占空比测量
	/*
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	*/
	TIM_PWMIConfig(TIM3,&TIM_ICInitStructure);
	
	
	//选择触发
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3,ENABLE);
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / TIM_GetCapture1(TIM3);
}

uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / TIM_GetCapture1(TIM3);
	
	
}




