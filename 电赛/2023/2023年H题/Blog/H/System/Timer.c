#include "stm32f10x.h"                  // Device header

extern uint16_t number;


void Timer_Init(void)
{
	//�����ڲ�ʱ��Դ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//���ö�ʱ��
	TIM_InternalClockConfig(TIM2);
	
	//�ṹ�嶨��
	TIM_TimeBaseInitTypeDef TIM_TimeBaesInitStructure;
	TIM_TimeBaesInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaesInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaesInitStructure.TIM_Period = 100-1;
	TIM_TimeBaesInitStructure.TIM_Prescaler = 720-1;
	TIM_TimeBaesInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaesInitStructure);
	
	//�����������־λ����������
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//���ü������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//NVIC������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	
	//ʹ�ܼ�����
	TIM_Cmd(TIM2,ENABLE);
	
}

/*
//�жϺ���ģ��
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		number ++;
		
		//�����������־λ������һֱ�����ж���
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}
*/




