#include "stm32f4xx.h"
void GPIO_Config(void) {
	
	// USART2_TX PD5 USART2_RX PD6 
	// USART2_CTS PD3 USART2_RTS PD4
	
	/* 一、开时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
	/* 二、定义结构体 */
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 三、配置结构体 */
	// TX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* 四、调用初始化函数，写入寄存器 */
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	//RX 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	// Input Capture
	// PB6~PB9 -> TIM4CH1~CH4
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

void GPIO_SET(void) {
	GPIO_SetBits(GPIOD,GPIO_Pin_5);
}

void GPIO_RESET(void) {
	GPIO_ResetBits(GPIOD,GPIO_Pin_5);
}
