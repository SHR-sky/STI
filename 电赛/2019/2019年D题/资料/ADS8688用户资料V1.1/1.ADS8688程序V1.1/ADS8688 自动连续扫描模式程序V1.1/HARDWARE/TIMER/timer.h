#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern uint16_t value[8];
extern uint8_t get_ok;//ADC数据读取完成


void TIM4_Int_Init(u16 arr,u16 psc); 
 
#endif
