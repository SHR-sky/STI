#ifndef _SYS_TICK_H_
#define _SYS_TICK_H_

#include "stm32f4xx.h"
#define SYS_TICK_UNIT 100 // 中断时间单元 单位：us

void SysTick_Delay_Us( __IO uint32_t us);
void Delay_ms(uint32_t msTime);
void Delay_s(uint32_t sTime);
#endif
