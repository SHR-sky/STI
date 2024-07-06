 #include "ADC.h"

int adcvalue1,adcvalue2,adcvalue3;
int adcvota ;
int i,sum,avgvota;

// ADC1 - PA1 ch1 fre
// ADC2 - PC1 ch11 phi
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	GPIO_InitTypeDef gpio_InitTypeDef;
	
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//开启ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2|RCC_APB2Periph_ADC1, ENABLE);
	
	//开启GPIO时钟 PA5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE); 
	
	
	//GPIO初始化 初始化为模拟功能
	gpio_InitTypeDef.GPIO_Pin = GPIO_Pin_1;
	gpio_InitTypeDef.GPIO_Mode = GPIO_Mode_AN;
	gpio_InitTypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio_InitTypeDef);
	GPIO_Init(GPIOA, &gpio_InitTypeDef);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2|RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2|RCC_APB2Periph_ADC1,DISABLE);	//复位结束	
	
	
	//初始化ADC_CCR寄存器
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//初始化ADC1
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC2, &ADC_InitStruct);
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//使能ADC
	ADC_Cmd(ADC2, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	/*
	ADC_InitTypeDef ADC_InitStructure; 
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// PC1 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE );	  //使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2,ENABLE); // ADC1,ADC2
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //不用DMA
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  //独立采样
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;  //速度尽可能低8分频
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2,DISABLE);	//复位结束	
	
	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//ADC1配置
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_Init(ADC2,&ADC_InitStructure);
    
	ADC_Cmd(ADC1,ENABLE);
	ADC_Cmd(ADC2,ENABLE);
	*/
}				  

//获得ADC值
//ch:通道值 0~3
uint16_t Get_Adc(ADC_TypeDef* kADCx,u8 ch)   
{
	ADC_RegularChannelConfig(kADCx,ch,1,ADC_SampleTime_144Cycles);
	ADC_SoftwareStartConv(kADCx);
	while(ADC_GetFlagStatus(kADCx, ADC_FLAG_EOC) == RESET);

	//读取ADC的值
	//return  ADC_GetConversionValue(ADC1);
	return ADC_GetConversionValue(kADCx); //返回最近一次ADC1规则组的转换结果
}

uint16_t Get_Adc_Average(ADC_TypeDef* kADCx,u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(kADCx,ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 
