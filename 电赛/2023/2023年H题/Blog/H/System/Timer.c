#include "stm32f10x.h"                  // Device header

extern uint16_t number;


void Timer_Init(void)
{
	//配置内部时钟源
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置定时器
	TIM_InternalClockConfig(TIM2);
	
	//结构体定义
	TIM_TimeBaseInitTypeDef TIM_TimeBaesInitStructure;
	TIM_TimeBaesInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaesInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaesInitStructure.TIM_Period = 100-1;
	TIM_TimeBaesInitStructure.TIM_Prescaler = 720-1;
	TIM_TimeBaesInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaesInitStructure);
	
	//清零计数器标志位，避免多计数
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//配置计数器中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//NVIC的配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	
	//使能计数器
	TIM_Cmd(TIM2,ENABLE);
	
}

/*
//中断函数模板
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		number ++;
		
		//清除计数器标志位，避免一直处于中断中
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}
*/




