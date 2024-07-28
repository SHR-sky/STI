#ifndef __ADCDMA_H__
#define __ADCDMA_H__

#include "sys.h"

/**
 * @brief Driver for ADC DMA USING TIM3
 * @details PB1 - ADC1_IN9      PC1 - ADC2-IN11
 * @author Rufish
 * @date 2024.07.27
 * @copyright Copyright (c)
 **/
 
/*
	ADC_GPIO_Init();
	TIM3_Config();
	ADC_Config();
	ADC1_DMA_Trig(ADC1_DMA_Size);
	delay_ms(200);  // NEED TO CHANGE
*/ 

// #define ADC1_DMA_Size  25000 //采样点数
#define ADC1_DMA_Size 1024 // 采样点数
#define ADC2_DMA_Size 1024 // 采样点数

void ADC_GPIO_Init(void);
void TIM3_Config(void);
void ADC_Config(void);
void ADC1_DMA_Trig(u16 Size);
void ADC2_DMA_Trig(u16 Size);

#endif
