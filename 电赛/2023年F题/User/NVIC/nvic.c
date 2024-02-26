#include "nvic.h"

void Set_NVIC_Init(void) {
	// 初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	// 设置中断EXTI0_IRQn 对应PA0
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	// 设置中断EXTI1_IRQn 对应PB1
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	// 设置中断EXTI2_IRQn 对应PC2
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	// 设置中断EXTI3_IRQn 对应PD3
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}