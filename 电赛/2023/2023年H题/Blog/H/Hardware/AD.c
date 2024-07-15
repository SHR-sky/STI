#include "AD.h"

///////ADC�ĳ�ʼ������/////////
void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;//����ADC�ṹ��
  //DMA_InitTypeDef DMA_InitStructure;//����DMA�ṹ��
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1| RCC_APB2Periph_GPIOA, ENABLE ); //ʹ��ADC1��GPIOAʱ��
  /*��ΪADC1��6ͨ��ģ�������GPIO��ʼ������*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //����ADC��Ƶ����4 56M/4=14,ADC���ʱ�䲻�ܳ���14M
  /*����ΪADC1������*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1�����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ģ��ת��������ɨ��ģʽ����ͨ����
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ģ��ת������������ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;//ת����ADCͨ������ĿΪ1
  ADC_Init(ADC1, &ADC_InitStructure);//Ҫ�����²�����ʼ��ADC_InitStructure
  //ת��ʱ��Tconv=����ʱ��+12.5������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_1Cycles5);
  /*ʹ��ADC1��DMA���䷽ʽ*/
  ADC_DMACmd(ADC1, ENABLE);
  /*ʹ��ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /*����ADC1��У׼�Ĵ��� */
  ADC_ResetCalibration(ADC1);
  /*��ȡADC����У׼�Ĵ�����״̬*/
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1); /*��ʼУ׼ADC1*/
  while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ADC1���ת��
}

extern short ADCConvertedValue[1000];

///////ADC��DMA��������////
void ADC_DMA_Config()
{
   /*DMA1��ͨ��1����*/
   DMA_InitTypeDef DMA_InitStructure;
   DMA_DeInit(DMA1_Channel1);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (ADC1->DR);//�����Դͷ��ַ
   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;//Ŀ���ַ
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //������Դͷ
   DMA_InitStructure.DMA_BufferSize = 1024;//���ݳ���Ϊ1024������㷨��1000�㣬FFT��1024��
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ���������
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���贫�����ֽ�Ϊ��λ
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ����ֽ�Ϊ��λ
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//��ѭ��
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4���ȼ�֮һ��(������)
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //���ڴ浽�ڴ�
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);//�������ϲ�����ʼ��DMA_InitStructure
   DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��DMA1
 }