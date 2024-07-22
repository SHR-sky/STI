// TIM4
#include "TIM.h"

#define ARR_NUM 250
#define PSC_NUM 84

void Timer_Init(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	// -------------------------------
	// TIM 2 定时500us自动中断
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_TimeBaseStructure.TIM_Period = (ARR_NUM-1); 
    TIM_TimeBaseStructure.TIM_Prescaler =(PSC_NUM-1); 
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