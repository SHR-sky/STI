#include "sys.h"

extern u16 ADC1_ConvertedValue[ ADC1_DMA_Size ];
extern u16 ADC2_ConvertedValue[ ADC2_DMA_Size ];

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Serial_Init();
	Serial_Printf("Init Finish!\r\n");
	
	ADS8688_Init(CH0_EN|CH1_EN|CH2_EN|CH3_EN);
	Set_CH_Range(CHIR_0,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_1,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_2,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_3,ADS8688_IR_N2_5V);
	MAN_CH_Mode(MAN_CH_0);					//选择通道
	Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());
	
	while(1)
	{
		delay_s(1);
		MAN_CH_Mode(MAN_CH_0);					//选择通道
		Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());
	}
}

