#include "DFPlayer.h"
#include "stdarg.h"

u8 cmd[10];
u8 volumn = 10;

void DFPlayer_Init(void)
{
	// USART1_TX PA9 USART2_RX PA10

	/*open the clock*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* define the struct */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* 三、配置结构体 */
	
	// TX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	// GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* 四、调用初始化函数，写入寄存器 */
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	// 发送STM32数据

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	USART_Cmd(USART1, ENABLE);
}

void DFPlayer_SendByte(u8 c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void DFPlayer_VolumnUp(void)
{
	if(volumn >= 30)
	{
		volumn = 30;
	}
	else
	{
		volumn += 1;
	}
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x06;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = volumn;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_VolumnDown(void)
{
	if(volumn <= 0)
	{
		volumn = 0;
	}
	else
	{
		volumn -= 1;
	}
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x06;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = volumn;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_VolumnSet(u8 v)
{
	if(v >= 30)
	{
		v = 30;
	}
	if(v<=0)
	{
		v = 0;
	}
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x06;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = v;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();	
}

void DFPlayer_PlayNext(void)
{
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x01;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = 0x00;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_PlayPre(void)
{
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x02;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = 0x00;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_Pause(void)
{
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x0e;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = 0x00;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_Play(void)
{
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x0d;
	// FEEDBACK
	cmd[4] = 0x00;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = 0x00;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_Reset(void)
{
	// HEAD
	cmd[0] = 0x7e;
	// VERSION
	cmd[1] = 0xff;
	// LEN
	cmd[2] = 0x06;
	// CMD
	cmd[3] = 0x0c;
	// FEEDBACK
	cmd[4] = 0x0d;
	// PARA1
	cmd[5] = 0x00;
	// PARA2
	cmd[6] = 0x00;
	DFPlayer_Checksum();
	cmd[9] = 0xef;
	DFPlayer_SendCMD();
}

void DFPlayer_Checksum(void)
{
	uint16_t checksum = 0;
	for (int i=1; i<7; i++) {
		checksum += cmd[i];
	}
	uint16_t leftsum = 0x010000 - checksum;
	cmd[7] = (uint8_t)(leftsum >> 8);
	cmd[8] = (uint8_t)(leftsum);
}

void DFPlayer_SendCMD(void)
{
	for(int i=0; i<10; i++)
	{
		DFPlayer_SendByte(cmd[i]);
		for(int j=0; j<100; j++);
	}
}

void DFPlayer_SendString(char *String)
{
	for (int i = 0; String[i] != '\0'; i++)
	{
		DFPlayer_SendByte(String[i]);
	}
}

void DFPlayer_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	DFPlayer_SendString(String);
}

void USART1_IRQHandler(void)
{
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
