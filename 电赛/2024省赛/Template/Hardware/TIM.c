// TIM4
#include "TIM.h"

#define ARR_NUM 10000
#define PSC_NUM 84

void Timer_Init(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	// -------------------------------
	// TIM 2 定时15Hz自动中断
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_TimeBaseStructure.TIM_Period = (84-1); 
    TIM_TimeBaseStructure.TIM_Prescaler =0x0; 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);          
 
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);  
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
    NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2,DISABLE); // 先不开启 
}

void TIM_InputCapture_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InternalClockConfig(TIM4); // choose internal clock

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ARR_NUM - 1;	// ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSC_NUM - 1; // PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;	// Advanced TIM
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	// IC1 PB6
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xf;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);

	// IC2 PB7
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);

	// IC3 PB8
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);

	// IC4 PB9
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

/*
void TIM4_IRQHandler(void)
{
	if (TIM_GetFlagStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC1) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC2) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC3) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC4) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		TIM_GetCapture4(TIM4); // 以CCR1为基准
	}
}
*/
