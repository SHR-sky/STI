#include "DA.h"


///////DAC1的DMA传输配置////
void DAC1_DMA_Config(short* ROM_TBL)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//使能DMA2时钟
	
	DMA_InitTypeDef DMA_InitStructure;
	
     //DMA2通道3配置
    DMA_DeInit(DMA2_Channel3); //根据默认设置初始化DMA2
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(DAC->DHR12R2);//外设地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ROM_TBL;//内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设DAC作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize =200;//数据长度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设传输以半字为单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存以半字为单位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4优先级之一的(高优先级)
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//非内存到内存
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);//根据以上参数初始化DMA_InitStructure
    //使能DAC的DMA中断
  DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);//配置控制DAC的DMA通道传输完成中断
    //使能DMA2的通道3
    DMA_Cmd(DMA2_Channel3, ENABLE);
}

///////DAC2的DMA传输配置////
void DAC2_DMA_Config(short* ROM_TBL)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//使能DMA2时钟
	DMA_InitTypeDef DMA_InitStructure;
     //DMA2通道4配置 
    DMA_DeInit(DMA2_Channel4); //根据默认设置初始化DMA2
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (DAC->DHR12R2);//外设地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ROM_TBL;//内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设DAC作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize =200;//数据长度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设传输以半字为单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存以半字为单位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4优先级之一的(高优先级)
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//非内存到内存
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);//根据以上参数初始化DMA_InitStructure
    //使能DAC的DMA中断
  DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);//配置控制DAC的DMA通道传输完成中断     
    //使能DMA2的通道4
    DMA_Cmd(DMA2_Channel4, ENABLE);    
}


void DAC_Config(void)
{
    DAC_InitTypeDef  DAC_InitStructure;//库函数定义DAC结构体
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO结构体
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//DAC时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//使能定时器时钟
    /*将GPIO配置为DAC的模拟复用功能*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  //////DAC通道1由基本定时器6触发////////////
  /* TIM6配置*/
  TIM_PrescalerConfig(TIM6,1-1,TIM_PSCReloadMode_Update);//设置TIM6预分频值
  TIM_SetAutoreload(TIM6, 56-1);//设置定时器计数器值
  /* TIM6触发模式选择，这里为定时器2溢出更新触发*/
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;//定时器6触发
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//无波形产生
  DAC_InitStructure.DAC_OutputBuffer= DAC_OutputBuffer_Disable;//DAC_OutputBuffer_Enable;//不使能输出缓存
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);//根据以上参数初始化DAC结构体
  //////DAC通道2由基本定时器7触发////////////
  /* TIM7配置*/
  TIM_PrescalerConfig(TIM7,1-1,TIM_PSCReloadMode_Update);//设置TIM7预分频值
  TIM_SetAutoreload(TIM7, 56-1);//设置定时器计数器值
  /* TIM7触发模式选择，这里为定时器2溢出更新触发*/
  TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T7_TRGO;//定时器7触发
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//无波形产生
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//DAC_OutputBuffer_Enable;//不使能输出缓存
  /*DAC通道2配置*/
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);//根据以上参数初始化DAC结构体
    /* 使能DAC通道1*/
  DAC_Cmd(DAC_Channel_1, ENABLE);
  //使能DAC通道1的DMA
  DAC_DMACmd(DAC_Channel_1, ENABLE);
    /* 使能DAC通道2*/
  DAC_Cmd(DAC_Channel_2, ENABLE);
  //使能DAC通道2的DMA
  DAC_DMACmd(DAC_Channel_2, ENABLE);
//使能定时器6和7
  TIM_Cmd(TIM6, ENABLE);
  TIM_Cmd(TIM7, ENABLE);
}
