#include "gpio.h"

void InputGPIO_Init(void) {
	// 使用PA0,PB1,PC2,PD3作为四个中断输入来源
	
	// 开启GPIOA~D的时钟
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	// 初始化PA0
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// 初始化PB1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// 初始化PC2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	// 初始化PD3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
}
