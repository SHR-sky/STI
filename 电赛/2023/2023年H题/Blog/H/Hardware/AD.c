#include "AD.h"

///////ADC的初始化配置/////////
void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;//定义ADC结构体
  //DMA_InitTypeDef DMA_InitStructure;//定义DMA结构体
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA1时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1| RCC_APB2Periph_GPIOA, ENABLE ); //使能ADC1及GPIOA时钟
  /*作为ADC1的6通道模拟输入的GPIO初始化配置*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //设置ADC分频因子4 56M/4=14,ADC最大时间不能超过14M
  /*下面为ADC1的配置*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//模数转换工作在扫描模式（多通道）
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//模数转换工作在连续模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;//转换的ADC通道的数目为1
  ADC_Init(ADC1, &ADC_InitStructure);//要把以下参数初始化ADC_InitStructure
  //转换时间Tconv=采样时间+12.5个周期
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_1Cycles5);
  /*使能ADC1的DMA传输方式*/
  ADC_DMACmd(ADC1, ENABLE);
  /*使能ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /*重置ADC1的校准寄存器 */
  ADC_ResetCalibration(ADC1);
  /*获取ADC重置校准寄存器的状态*/
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1); /*开始校准ADC1*/
  while(ADC_GetCalibrationStatus(ADC1)); //等待校准完成
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能ADC1软件转换
}

extern short ADCConvertedValue[1000];

///////ADC的DMA传输配置////
void ADC_DMA_Config()
{
   /*DMA1的通道1配置*/
   DMA_InitTypeDef DMA_InitStructure;
   DMA_DeInit(DMA1_Channel1);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (ADC1->DR);//传输的源头地址
   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;//目标地址
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设作源头
   DMA_InitStructure.DMA_BufferSize = 1024;//数据长度为1024，卷积算法用1000点，FFT用1024点
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不递增
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设传输以字节为单位
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存以字节为单位
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//不循环
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4优先级之一的(高优先)
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //非内存到内存
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根据以上参数初始化DMA_InitStructure
   DMA_Cmd(DMA1_Channel1, ENABLE);//使能DMA1
 }