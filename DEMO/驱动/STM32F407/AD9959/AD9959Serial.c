#include "AD9959Serial.h"

void AD9959_Serial_Init(void)
{
    // USART3_TX PD8 USART3_RX PD9

    /*open the clock*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

    /* define the struct */
    GPIO_InitTypeDef GPIO_InitStruct;

    /* 三、配置结构体 */

    // TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    // GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    /* 四、调用初始化函数，写入寄存器 */
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // 发送STM32数据

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART3, ENABLE);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStruct);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(USART3, ENABLE);
}

void AD9959_Init(void)
{
    AD9959_Printf("*Init_AD9959()");
}

void AD9959_Reset(void)
{
    AD9959_Printf("*InitReset()");
}

void AD9959_WriteFre(u8 ch, u32 fre)
{
    AD9959_Printf("*Write_frequency(%d,%d)", ch, fre);
}

void AD9959_WriteAmp(u8 ch, u32 amp)
{
    AD9959_Printf("*Write_Amplitude(%d,%d)", ch, amp);
}

void AD9959_Sweep(u8 ch, u32 Start_freq, u32 End_freq, u32 step, u32 unitTime)
{
    for (u32 i = Start_freq; i <= End_freq; i += step)
    {
        AD9959_WriteFre(ch, i);
        delay_us(unitTime);
    }
}

void AD9959_SendByte(u8 c)
{
    USART_SendData(USART3, c);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;
}

void AD9959_SendString(char *String)
{
    for (int i = 0; String[i] != '\0'; i++)
    {
        AD9959_SendByte(String[i]);
    }
}

void AD9959_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    AD9959_SendString(String);
}

void USART3_IRQHandler(void)
{
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}