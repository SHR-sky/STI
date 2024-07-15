#include "DA.h"


///////DAC1��DMA��������////
void DAC1_DMA_Config(short* ROM_TBL)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//ʹ��DMA2ʱ��
	
	DMA_InitTypeDef DMA_InitStructure;
	
     //DMA2ͨ��3����
    DMA_DeInit(DMA2_Channel3); //����Ĭ�����ó�ʼ��DMA2
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(DAC->DHR12R2);//�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ROM_TBL;//�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//����DAC��Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize =200;//���ݳ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ���������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���贫���԰���Ϊ��λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ��԰���Ϊ��λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4���ȼ�֮һ��(�����ȼ�)
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ�
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);//�������ϲ�����ʼ��DMA_InitStructure
    //ʹ��DAC��DMA�ж�
  DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);//���ÿ���DAC��DMAͨ����������ж�
    //ʹ��DMA2��ͨ��3
    DMA_Cmd(DMA2_Channel3, ENABLE);
}

///////DAC2��DMA��������////
void DAC2_DMA_Config(short* ROM_TBL)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//ʹ��DMA2ʱ��
	DMA_InitTypeDef DMA_InitStructure;
     //DMA2ͨ��4���� 
    DMA_DeInit(DMA2_Channel4); //����Ĭ�����ó�ʼ��DMA2
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (DAC->DHR12R2);//�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ROM_TBL;//�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//����DAC��Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize =200;//���ݳ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ���������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���贫���԰���Ϊ��λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ��԰���Ϊ��λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4���ȼ�֮һ��(�����ȼ�)
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ�
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);//�������ϲ�����ʼ��DMA_InitStructure
    //ʹ��DAC��DMA�ж�
  DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);//���ÿ���DAC��DMAͨ����������ж�     
    //ʹ��DMA2��ͨ��4
    DMA_Cmd(DMA2_Channel4, ENABLE);    
}


void DAC_Config(void)
{
    DAC_InitTypeDef  DAC_InitStructure;//�⺯������DAC�ṹ��
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO�ṹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//DACʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//ʹ�ܶ�ʱ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//ʹ�ܶ�ʱ��ʱ��
    /*��GPIO����ΪDAC��ģ�⸴�ù���*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  //////DACͨ��1�ɻ�����ʱ��6����////////////
  /* TIM6����*/
  TIM_PrescalerConfig(TIM6,1-1,TIM_PSCReloadMode_Update);//����TIM6Ԥ��Ƶֵ
  TIM_SetAutoreload(TIM6, 56-1);//���ö�ʱ��������ֵ
  /* TIM6����ģʽѡ������Ϊ��ʱ��2������´���*/
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;//��ʱ��6����
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//�޲��β���
  DAC_InitStructure.DAC_OutputBuffer= DAC_OutputBuffer_Disable;//DAC_OutputBuffer_Enable;//��ʹ���������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);//�������ϲ�����ʼ��DAC�ṹ��
  //////DACͨ��2�ɻ�����ʱ��7����////////////
  /* TIM7����*/
  TIM_PrescalerConfig(TIM7,1-1,TIM_PSCReloadMode_Update);//����TIM7Ԥ��Ƶֵ
  TIM_SetAutoreload(TIM7, 56-1);//���ö�ʱ��������ֵ
  /* TIM7����ģʽѡ������Ϊ��ʱ��2������´���*/
  TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T7_TRGO;//��ʱ��7����
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//�޲��β���
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//DAC_OutputBuffer_Enable;//��ʹ���������
  /*DACͨ��2����*/
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);//�������ϲ�����ʼ��DAC�ṹ��
    /* ʹ��DACͨ��1*/
  DAC_Cmd(DAC_Channel_1, ENABLE);
  //ʹ��DACͨ��1��DMA
  DAC_DMACmd(DAC_Channel_1, ENABLE);
    /* ʹ��DACͨ��2*/
  DAC_Cmd(DAC_Channel_2, ENABLE);
  //ʹ��DACͨ��2��DMA
  DAC_DMACmd(DAC_Channel_2, ENABLE);
//ʹ�ܶ�ʱ��6��7
  TIM_Cmd(TIM6, ENABLE);
  TIM_Cmd(TIM7, ENABLE);
}
