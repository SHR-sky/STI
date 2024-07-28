// TIM4
#include "Serial.h"
#include "TODA.h"
#include "math.h"

#define ARR_NUM 10000
#define PSC_NUM 84
#define SCALE 1240
#include "stm32f4xx.h"

int triggerOrder = 0;
int nowTime = 0;
int triggerTime[4];
int triggerTimeCCR[4];
int t[3];

void TIM_Init(void)
{
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

int diffTime = 0;
int area;

int s1,s2;
int x_AB[2];
int y_12[2];
int cnt = 0;
void TIM4_IRQHandler(void)
{
	if (TIM_GetFlagStatus(TIM4, TIM_IT_Update) == SET)
	{
		nowTime++;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		nowTime %= 100000000;
		if(cnt>=20)
		{
			cnt = 0;
			triggerOrder = 0;
			//Serial_Printf(" Clear! ");
		}
		if(triggerOrder!=0)
		{
			cnt++;
		}
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC1) == SET)
	{
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		triggerTime[0] = nowTime; // 单位：10ms
		triggerTimeCCR[0] = TIM_GetCapture1(TIM4); // 以CCR1为基准 单位：1us
		//Serial_Printf("CC1-%d0ms,%dus\n \n", triggerTime[0],triggerTimeCCR[0]);
		triggerOrder++;
		
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC2) == SET)
	{
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		triggerTime[1] = nowTime;
		triggerTimeCCR[1] = TIM_GetCapture2(TIM4); // 以CCR1为基准
		//Serial_Printf("CC2-%d0ms,%dus\n \n", triggerTime[1],triggerTimeCCR[1]);
		t[0] = triggerTime[1]*10+triggerTimeCCR[1];
		triggerOrder++;
		if(triggerOrder % 3 == 0)
		{
			nowTime = 0;
			triggerOrder = 0;
			TODA((t[1]-t[0])*1.0/SCALE,(t[2]-t[0])*1.0/SCALE, x_AB, y_12);
			//Serial_Printf("  %d %d   \n", t[1]-t[0],t[2]-t[0]);
			//Serial_Printf("(%c%c,%2d%2d  )",x_AB[0],x_AB[1],y_12[0],y_12[1]);
		}
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC3) == SET)
	{
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		triggerTime[2] = nowTime;
		triggerTimeCCR[2] = TIM_GetCapture3(TIM4); // 以CCR1为基准
		//Serial_Printf("CC3-%d0ms,%dus\n \n", triggerTime[2],triggerTimeCCR[2]);
		t[1] = triggerTime[2]*10+triggerTimeCCR[2];
		triggerOrder++;
		if(triggerOrder % 3 == 0)
		{
			nowTime = 0;
			triggerOrder = 0;
			TODA((t[1]-t[0])*1.0/SCALE,(t[2]-t[0])*1.0/SCALE, x_AB, y_12);
			//Serial_Printf("  %d %d   \n", t[1]-t[0],t[2]-t[0]);
			//Serial_Printf("(%c%c,%2d%2d  )",x_AB[0],x_AB[1],y_12[0],y_12[1]);
		}
	}
	if (TIM_GetFlagStatus(TIM4, TIM_IT_CC4) == SET)
	{
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		triggerTime[3] = nowTime;
		triggerTimeCCR[3] = TIM_GetCapture4(TIM4); // 以CCR1为基准
		//Serial_Printf("CC4-%d0ms,%dus\n \n", triggerTime[3],triggerTimeCCR[3]);
		t[2] = triggerTime[3]*10+triggerTimeCCR[3];
		triggerOrder++;
		if(triggerOrder % 3 == 0)
		{
			nowTime = 0;
			triggerOrder = 0;
			TODA((t[1]-t[0])*1.0/SCALE,(t[2]-t[0])*1.0/SCALE, x_AB, y_12);
			//Serial_Printf("  %d %d   \n", t[1]-t[0],t[2]-t[0]);
			//Serial_Printf("(%c%c,%2d%2d  )",x_AB[0],x_AB[1],y_12[0],y_12[1]);
		}
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

#define SQUARE_TIME 500
