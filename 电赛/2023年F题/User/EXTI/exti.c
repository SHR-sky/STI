#include "exti.h"
#include "gpio.h"
#include "nvic.h"

#define GPIOA_IRQHandler EXTI0_IRQHandler
#define GPIOB_IRQHandler EXTI1_IRQHandler
#define GPIOC_IRQHandler EXTI2_IRQHandler
#define GPIOD_IRQHandler EXTI3_IRQHandler

void Project_EXTI_Init(void)
{
	// 使用EXTI0~3作为4个的中断输入，分布对应PA0,PB1,PC2,PD3
	
	// 外部中断需使能SYSCFG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// GPIO初始化
	InputGPIO_Init();
	// NVIC初始化
	Set_NVIC_Init();
	
	// 连接EXIT0到GPIOA,PA0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	
	// 连接EXIT1到GPIOB,PB1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource1);
	
	// 连接EXIT2到GPIOC,PC2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource2);
	
	// 连接EXIT3到GPIOD,PD3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource3);
	
	
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	
	// 初始化EXTI0
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStruct);
	
	// 初始化EXTI1
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStruct);
	
	// 初始化EXTI2
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_Init(&EXTI_InitStruct);
	
	// 初始化EXTI3
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_Init(&EXTI_InitStruct);
}

void GPIOA_IRQHandler(void) {

}

void GPIOB_IRQHandler(void) {

}

void GPIOC_IRQHandler(void) {

}

void GPIOD_IRQHandler(void) {

}