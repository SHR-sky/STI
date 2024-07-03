 #include "ADC.h"

int adcvalue1,adcvalue2,adcvalue3;
int adcvota ;
int i,sum,avgvota;

//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
 
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //不用DMA
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  //独立采样
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;  //速度尽可能低8分频
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//ADC1配置
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
    
	ADC_Cmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_144Cycles);

	ADC_SoftwareStartConv(ADC1);
}				  

//获得ADC值
//ch:通道值 0~3
uint16_t Get_Adc(u8 ch)   
{
	
	adcvalue1 = ADC_GetConversionValue(ADC1);
    adcvota = adcvalue1 *3300 / 0xfff;
    
    for(i = 0;i<10000;i++)
    {
		sum += adcvota;
		if(i ==9999)
		{
			avgvota = sum/10000;
			sum = 0;
		}
	}
	return ADC_GetConversionValue(ADC1); //返回最近一次ADC1规则组的转换结果
}

uint16_t Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 
