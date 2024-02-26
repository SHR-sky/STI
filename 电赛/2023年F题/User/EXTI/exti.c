#include "exti.h"
#include "gpio.h"
#include "nvic.h"

#define GPIOA_IRQHandler EXTI0_IRQHandler
#define GPIOB_IRQHandler EXTI1_IRQHandler
#define GPIOC_IRQHandler EXTI2_IRQHandler
#define GPIOD_IRQHandler EXTI3_IRQHandler

void Project_EXTI_Init(void)
{
	// ʹ��EXTI0~3��Ϊ4�����ж����룬�ֲ���ӦPA0,PB1,PC2,PD3
	
	// �ⲿ�ж���ʹ��SYSCFG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// GPIO��ʼ��
	InputGPIO_Init();
	// NVIC��ʼ��
	Set_NVIC_Init();
	
	// ����EXIT0��GPIOA,PA0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	
	// ����EXIT1��GPIOB,PB1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource1);
	
	// ����EXIT2��GPIOC,PC2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource2);
	
	// ����EXIT3��GPIOD,PD3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource3);
	
	
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	
	// ��ʼ��EXTI0
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStruct);
	
	// ��ʼ��EXTI1
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStruct);
	
	// ��ʼ��EXTI2
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_Init(&EXTI_InitStruct);
	
	// ��ʼ��EXTI3
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