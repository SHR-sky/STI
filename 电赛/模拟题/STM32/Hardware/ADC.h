#ifndef __ADC_H
#define __ADC_H
#include "sys.h"

/**
 * @brief Driver for ADC for STM32F407
 * @details ADC3_IN4 PF6
 * @author Rufish
 * @date 2024.07.27
 * @copyright Copyright (c)
 **/

void Adc_Init(void);
uint16_t Get_Adc(ADC_TypeDef *kADCx, u8 ch);
uint16_t Get_Adc_Average(ADC_TypeDef *kADCx, u8 ch, u8 times);

#endif
