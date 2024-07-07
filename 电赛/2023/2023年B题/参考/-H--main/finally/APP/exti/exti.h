#ifndef _exti_H
#define _exti_H


#include "system.h"

void My_EXTI_Init(void);
uint8_t Deal_Phase(uint8_t phase_in);
void Show_Wavemessage(uint8_t kind_a,uint8_t kind_b,uint8_t fre_a,uint8_t fre_b);
void Draw_Wave(uint8_t kind_a,uint8_t kind_b,uint8_t fre_a,uint8_t fre_b,uint8_t size);

#endif
