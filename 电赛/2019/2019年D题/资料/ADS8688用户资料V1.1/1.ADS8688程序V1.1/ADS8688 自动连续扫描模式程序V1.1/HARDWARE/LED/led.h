#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "stm32f10x.h"

//LED端口定义
#define LED0 PCout(0)// PA8
#define LED1 PDout(2)// PD2	

void LED_Init(void);//初始化

		 				    
#endif
