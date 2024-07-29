#include "sys.h"

extern u16 ADC1_ConvertedValue[ ADC1_DMA_Size ];
extern u16 ADC2_ConvertedValue[ ADC2_DMA_Size ];

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Serial_Init();
	Serial_Printf("Init Finish!\r\n");
	
	AD9959_Serial_Init();
	AD9959_Reset();
	AD9959_Init();
	AD9959_WriteFre(0,17600);
	AD9959_WriteAmp(0,1000);
	AD9959_WritePha(0,0);
	AD9959_WriteFre(1,17600);

	AD9959_WriteAmp(1,800);
	AD9959_WritePha(1,0);
	AD9959_WriteFre(2,17600);
	AD9959_WriteAmp(2,700);
	AD9959_WritePha(2,0);
	AD9959_WriteFre(3,17600);
	AD9959_WriteAmp(3,700);
	AD9959_WritePha(3,1000);
	while(1)
	{
		delay_s(1);
		MAN_CH_Mode(MAN_CH_0);					//选择通道
		Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());
	}
}

