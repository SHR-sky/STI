#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	//ø™∆Ù ±÷”
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO≈‰÷√
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void LED_ON(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
}

void LED_OFF(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
}

