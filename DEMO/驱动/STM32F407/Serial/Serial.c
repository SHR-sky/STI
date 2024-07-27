#include "Serial.h"
#include "stdio.h"
#include "stdarg.h"

void Serial_Init(void)
{

	// USART2_TX PA2 USART2_RX PA3
	// USART2_TX PD5 USART2_RX PD6
	// USART2_CTS PD3 USART2_RTS PD4

	/*open the clock*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	/* define the struct */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* 三、配置结构体 */
	
	// TX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* 四、调用初始化函数，写入寄存器 */
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//GPIO_Init(GPIOD, &GPIO_InitStruct);

	// RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);

	// 发送STM32数据

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	USART_Cmd(USART2, ENABLE);

}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t length)
{
	for (int i = 0; i < length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	for (int i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}

int pow10[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

void Serial_SendNumber(uint32_t Number, uint8_t length)
{
	for (int i = 0; i < length; i++)
	{
		Serial_SendByte(Number / pow10[length - i - 1] + '0');
		Number = Number - Number / pow10[length - i - 1] * pow10[length - i - 1];
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_LookUpData(void)
{
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
		return USART_ReceiveData(USART2);
	return 0;
}


extern uint8_t mod_flag;
short Res;
void USART2_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	{
		Res = USART_ReceiveData(USART2);
		//Serial_Printf("%x \r\n",Res);
		Serial_SendByte(Res);
		if(Res==0x1)
		{
			mod_flag = 1;
		}
		else if(Res==0x2)
		{
			mod_flag = 2;
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

void Serial_End(void)
{
	Serial_SendByte(0xff);
	Serial_SendByte(0xff);
	Serial_SendByte(0xff);
}

// 中断发送串口数据
