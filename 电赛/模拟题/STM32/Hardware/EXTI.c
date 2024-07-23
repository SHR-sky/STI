#include "EXTI.h"

void exti_init(void)  //外部中断初始化函数
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//设置中断组为 0
	//设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	// PF0 接收
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;                          
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
