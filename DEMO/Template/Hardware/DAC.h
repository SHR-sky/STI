#ifndef __DAC_H__
#define __DAC_H__

#include "sys.h"

/**
 * @brief Driver for DAC DMA
 * @details PA4 - DAC1
 *          PA5 - DAC2
 * @author Rufish
 * @date 2024.07.27
 * @copyright Copyright (c)
 **/

/*
	for(int i=0; i<200; i++)
	{
		meDA1_Value[i] = ADC1_ConvertedValue[i];
	}
	
	DA1_Init();
	TIM4_Init();
	TIM_Cmd(TIM4, ENABLE);
	
	DA2_Init();
	TIM6_Init();
	TIM_Cmd(TIM6, ENABLE);
*/

// ！！！需TIM触发转换，请额外配置TIM
#define DA1_Value_Length 200
#define DA2_Value_Length 200

void DA1_Init(void);
void DA2_Init(void);

void TIM4_Init(void);
void TIM6_Init(void);

// uint16_t DA1_GetValue();
// uint16_t DA2_GetValue();

#endif
