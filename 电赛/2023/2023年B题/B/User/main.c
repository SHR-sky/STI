#include "sys.h"
#include "math.h"

#define pi 3.1415926

uint8_t flag = 0;

float length = 0;
uint8_t index = 0;
float length_result[100];

float Av;
float phi;
float de_phi;

float fre;

float C;

void Mea_Length(void);
float Cal_Length(float lambda);
// void Cal_phase(void);

void Mea_C(void);
void Cal_C(void);

int main(void)
{
	// 初始化
	Adc_Init();
	ADF4351Init();

	// 接入10.5m 就行标定
	ADF4351WriteFreq(90.0); // 20.0MHZ 对应波长 10.5m 可测量最长2.625m

	while (1)
	{
		if (flag == 1)
		{
			Serial_SendByte(0x11); // 使串口屏显示测量中

			Mea_Length();

			Serial_SendByte(0x22); // 使串口屏显示结果
			flag = 0;
		}
		if (flag == 2)
		{
			Serial_SendByte(0x11); // 使串口屏显示测量中

			Mea_C();

			Serial_SendByte(0x33); // 使串口屏显示结果
			flag = 0;
		}
	}
}

void TIM2_IRQHandler(void)
{
}

// 中断接收串口数据
void USART3_IRQHandler(void)
{
	uint8_t Res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3); // 读取接收到的数据
		if (Res == 0x23)
			flag = 1;
		else if (Res == 0x45)
			flag = 2;
		else
			flag = 0;
	}
}

void Mea_Length(void)
{
	// DDS输入固定频率信号，首先粗估1次，确定频率，然后100次取平均

	// 10m以上
	ADF4351WriteFreq(2.5); // 2.5MHZ 对应波长 84m 可测量最长21m
	length = Cal_Length((float)84);
	if (length >= 10)
	{
		for (int i = 0; i < 100; i++)
		{
			length_result[i] = Cal_Length((float)84);
		}
	}
	else
	{
		// 10m以下，5m以上
		ADF4351WriteFreq(5.0); // 5.0MHZ 对应波长 42m 可测量最长10.5m
		length = Cal_Length((float)42);
		if (length >= 5)
		{
			for (int i = 0; i < 100; i++)
			{
				length_result[i] = Cal_Length((float)42);
			}
		}
		else
		{
			// 10m以下，5m以上
			ADF4351WriteFreq(10.0); // 10.0MHZ 对应波长 21m 可测量最长5.25m
			length = Cal_Length((float)21);
			if (length >= 2)
			{
				for (int i = 0; i < 100; i++)
				{
					length_result[i] = Cal_Length((float)21);
				}
			}
			else
			{
				// 2m以下
				ADF4351WriteFreq(20.0); // 20.0MHZ 对应波长 10.5m 可测量最长2.625m
				for (int i = 0; i < 100; i++)
				{
					length_result[i] = Cal_Length((float)10.5);
				}
			}
		}
	}
}

float Cal_Length(float lambda)
{
	float angel = GetPhs();
	float result = (angel * lambda) / (float)(360 * 2);
	return result;
}

void Mea_C(void)
{
	// DDS输入固定频率1信号

	// 角度计算

	// DDS输入固定频率2信号

	// 角度计算

	// DDS输入固定频率3信号

	// 比较

	// 电容测量100次，队列取平均
}

const float R0 = 6.6; // 6.6M
float Cf = 0; 


void Cal_C(void)
{
	de_phi = GetPhs();
	Av = GetMag();

	Cf = (float)1.44 + (float)95.27 * length;
	C = sqrt( (R0/Av)* (R0/Av) / (float)2 * pi * fre ) - Cf;

}
