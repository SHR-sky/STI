#include "stm32f4xx.h"
#include <stdio.h>

void get_time_de(uint32_t time1, uint32_t time2, uint32_t time3);
void exchange(uint32_t *a, uint32_t *b);


// 全局变量，存储三个个上升沿触发的时间戳
volatile uint32_t time1 = 0;
volatile uint32_t time2 = 0;
volatile uint32_t time3 = 0;

//全局变量，时间差
uint32_t s1;
uint32_t s2;

int main(void)
{
    // 初始化定时器2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_Prescaler = 84 - 1; // 定时器时钟为84 MHz，84分频，得一微秒的分辨率
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 0xFFFFFFFF; // 设定定时器周期为最大值
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    TIM_Cmd(TIM2, ENABLE);

    // 配置用于检测第信号的引脚，三路输入，上升沿触发
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //结构体初始化定义
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 配置外部中断线0
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    // 配置中断
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 防跑飞
    while (1);
}

//数字交换
void exchange(uint32_t *a, uint32_t *b)
{
    uint32_t temp;
    temp = *a;
    *a = *b;
    *b = temp; 
}

//大小排序
void get_time_de(uint32_t time1, uint32_t time2, uint32_t time3)
{
    if(time1 > time2) exchange(time1, time2);
    if(time2 > time3) exchange(time2, time3);
    if(time1 > time2) exchange(time1, time2);

    s1 = time3 - time2;
    s2 = time2 - time1;

}


void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /*此处的gpio_read，我觉得要根据现实，加一点去抖的处理*/
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
        {
            // 检测到第一个脉冲信号
            time1 = TIM_GetCounter(TIM2);
        }

        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET)
        {
            // 检测到第二个脉冲信号
            time2 = TIM_GetCounter(TIM2);

        }
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == Bit_SET)
        {
            // 检测到第三个脉冲信号
            time2 = TIM_GetCounter(TIM2);

        }
        get_time_de(time1, time2, time3);
        //时间戳记录复位
        time1 = 0;
        time2 = 0;
        time3 = 0;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
