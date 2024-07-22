#include "sys.h"

// 发送字符串
u8 str[100];
// 解码字符串
u8 decodeStr[100];
u8 strIndex = 0;
u8 charIndex = 0;
u8 startIndex = 0;

#define INFO_SET PEout(5)
#define INFO_GET PEin(6)
#define HIGH_LOW_DIVISION 2000

void EncodeSendInfo(void); // 编码与发送
void DecodeInfo(void); // 解码

// 空闲拉高
// 2个bit低电平代表传输开始，10个bit高电平代表传输结束
// 每个字符以8bit传输
// 最多100个字符
// 共800 + 2 + 10 = 812个bit
// 一个定时器250us, 不超过0.25s可传送所有数据 共
// 一个定时器125us DMA 收集2000个点 0.25接收 解读数据 2个点为1bit
		
short ADCConvertedValue[ADC_DMA_Size]; // ADC采集数据 2000个点
		
int main()
{
	while(1)
	{
		if(INFO_GET==0)
		{
			ADC_DMA_Trig(ADC_DMA_Size); // 开始采样
			delay_us(300); // 等待采样完成
			DecodeInfo(); // 解码
		}
	}
}

void TIM2_IRQHandler(void) // 发送BIT
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
		if(startIndex<2)
		{
			startIndex += 1;
			INFO_SET = 0; // 连续2bit低电平
		}
		else
		{
			if(strIndex<100)
			{
				if(str[strIndex]=='\0')
				{
					INFO_SET = 1; // 结束传输 设为高电平
					return;
				}
				INFO_SET = (str[strIndex]&(1<<charIndex))>>charIndex; // 低位传输
				charIndex += 1;
				if(charIndex >= 8)
				{
					charIndex = 0;
					strIndex += 1;
				}
			}
			else
			{
				INFO_SET = 1; // 传输满 设为高电平
			}
		}
	}
}	

u8 recStartIndex = 0;
u8 recStrIndex = 0;
u8 recCharIndex = 0;
u8 recEndIndex = 1;
char val = 0;
u8 recBit = 0;
u8 preRecBit = 0;

void DecodeInfo(void)
{
	recStartIndex = 0;
	recStrIndex = 0;
	recCharIndex = 0;
	recEndIndex = 1;
	if(ADCConvertedValue[1]<HIGH_LOW_DIVISION&&ADCConvertedValue[3]<HIGH_LOW_DIVISION) // 开头两个低电平
	for(int i=5; i<2000; i+=2) // 步进为2
	{
		preRecBit = recBit;
		recBit = ADCConvertedValue[i]>HIGH_LOW_DIVISION?1:0;
		if(preRecBit==1&&recBit==1) // 连续1
		{
			recEndIndex += 1;
		}
		else 
		{
			recEndIndex = 1; //重新计数
		}
		if(recEndIndex>=10) // 连续10个1
		{
			decodeStr[recStrIndex-1] = '\0';
			recEndIndex = 1;
			break;
		}
		val += recBit<<recCharIndex;
		recCharIndex += 1;
		if(recCharIndex>=8) // 一个字符解码完成
		{
			recCharIndex = 0;
			decodeStr[recStrIndex] = val;
			val = 0;
			recStrIndex += 1;
		}
	}
}

void EncodeSendInfo(void)
{
	TIM2->CNT = 0;
	TIM_Cmd(TIM2,ENABLE);
	strIndex = 0; // 字符串位
	charIndex = 0; // 字符位 
	startIndex = 0; // 起始位
	delay_us(300); // 等待发送完成
	TIM_Cmd(TIM2,DISABLE); // 关闭计数器
	TIM2->CNT = 0;
}