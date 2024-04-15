#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/*---------------------- LED配置宏 ------------------------*/

#define LED1_PIN             GPIO_Pin_0       		 // LED1 引脚      
#define LED1_PORT            GPIOC                  // LED1 GPIO端口     
#define LED1_CLK             RCC_AHB1Periph_GPIOC	 // LED1 GPIO端口时钟

/*---------------------- LED控制宏 ------------------------*/
					
#define LED1_ON 	  GPIO_ResetBits(LED1_PORT,LED1_PIN)			// 输出低电平，点亮LED1	
#define LED1_OFF 	  GPIO_SetBits(LED1_PORT,LED1_PIN)				// 输出高电平，关闭LED1	
#define LED1_Toggle	GPIO_ToggleBits(LED1_PORT, LED1_PIN)		// LED状态翻转


/*---------------------- 函数声明 ----------------------------*/

void LED_Init(void);	//LED初始化函数

#endif //__LED_H

