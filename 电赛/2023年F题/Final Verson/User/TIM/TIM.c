// TIM4
#include "Serial.h"

#define ARR_NUM 2
#define PSC_NUM 2
#include "stm32f4xx.h"

int triggerOrder = 4;
int nowTime = 0;
struct TriggerTime
{
	int order;
	int time;
}triggerTime[4];

void TIM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_InternalClockConfig(TIM4); // choose internal clock
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ARR_NUM-1; // ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSC_NUM-1; // PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // Advanced TIM
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	// IC1 PB6
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xf;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	// IC2 PB7
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	// IC3 PB8
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	// IC4 PB9
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC3,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM4, TIM_IT_Update)==SET)
	{
		nowTime++;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		nowTime %= 100000000;
	}
	if(triggerOrder >= 3)
	{
		triggerOrder = 0;
		nowTime = 0;
	}
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC1)==SET)
	{
		Serial_Printf("Reveive from It1\n");
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		triggerTime[0].order = triggerOrder++;
		triggerTime[0].time = nowTime;
		Serial_Printf("CC1:%d\n",nowTime);
	}
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC2)==SET)
	{
		Serial_Printf("Reveive from It2\n");
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		triggerTime[1].order = triggerOrder++;
		triggerTime[1].time = nowTime;
		Serial_Printf("CC2:%d\n",nowTime);
	}
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC3)==SET)
	{
		Serial_Printf("Reveive from It3\n");
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		triggerTime[2].order = triggerOrder++;
		triggerTime[2].time = nowTime;
		Serial_Printf("CC3:%d\n",nowTime);
	}
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC4)==SET)
	{
		Serial_Printf("Reveive from It4\n");
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		triggerTime[3].order = triggerOrder++;
		triggerTime[3].time = nowTime;
		Serial_Printf("CC4:%d\n",nowTime);
	}
}

/*
void PWMSetCCRPercent(uint16_t percent)
{
	percent = percent % 101;
	uint16_t compare = percent*ARR_NUM/100;
	TIM_SetCompare3(TIM3, compare);
}
*/
