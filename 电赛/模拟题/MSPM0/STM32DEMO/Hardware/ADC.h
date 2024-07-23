#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void Adc_Init(void);
uint16_t Get_Adc(ADC_TypeDef* kADCx,u8 ch);
uint16_t Get_Adc_Average(ADC_TypeDef* kADCx,u8 ch,u8 times);
 
#endif 
