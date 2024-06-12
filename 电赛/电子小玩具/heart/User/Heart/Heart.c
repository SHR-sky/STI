#include "heart.h"

void Dac1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	/*ʹ��ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	DAC_InitStructure.DAC_Trigger= DAC_Trigger_None; //��ʹ�ô�������
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;//��DAC1�������
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);  //��ʼ�� DAC ͨ�� 1
	
	DAC_Cmd(DAC_Channel_1,ENABLE);//ʹ�� DAC1
	DAC_SetChannel1Data(DAC_Align_12b_R,0);//12λ�Ҷ���,����DAC��ʼֵ
}
 
//����ͨ�� 1 �����ѹ
//vol:0~3300,���� 0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp = vol;
	temp /= 1000;
	temp = temp * 4096 / 3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ�������DACֵ
}
