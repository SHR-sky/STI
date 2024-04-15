/***
	*************************************************************************************************
	*	@version V1.0
	*  @date    2023-5-16
	*	@author  反客科技	
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32F407VET6核心板 （型号：FK407M3-VET6）
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*	初始化LED的IO口，配置为推挽输出、速度等级2M。
	*
	************************************************************************************************
***/


#include "led.h"  

/*************************************************************************************************
*	函 数 名:	LED_Init
*
*	函数功能:	IO口初始化
*	 
*************************************************************************************************/

void LED_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 	//初始化GPIOG时钟	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度选择
	
	//初始化 LED1 引脚
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(LED1_PORT,LED1_PIN);  //PG7输出低电平
}




