#include "sys.h"
#include "math.h"

#define pi 3.1415926

uint8_t flag = 0;

const float R0 = 6.6; // 6.6M
float Cf = 0;

float length = 0;
float length_result[100];
float C_result[100];
float R_result[100];

float angle_direct = 0;

float Av;
float phi;
float de_phi;

float fre;

float C;
float R;
uint8_t CorR;

void Mea_Length(void);
float Cal_Length(float lambda);
// void Cal_phase(void);

void Mea_C(void);
void Cal_C(void);

uint8_t compare(float c1, float c2, float c3);
float average(float *num);
float myabs(float a);

int main(void)
{
	Serial_Init();
	Serial_Printf("Hi!");
	// 初始化
	Adc_Init();

	// 接入90MHZ，进行标定
	fre = 90.0;
	// AD9954_Set_Fre(fre);//设置AD9954输出频率，点频
	// AD9954_Set_Amp(16383);//写幅度
	// AD9954_Set_Phase(0);//写相位
	AD9959_Init();								//初始化控制AD9959需要用到的IO口,及寄存器
	AD9959_Set_Fre(CH3, 2625000);	//设置通道0频率100000Hz
	//AD9959_Set_Fre(CH3, 80000000);	//设置通道1频率100000Hz
		
	AD9959_Set_Amp(CH0, 1023); 		//设置通道0幅度控制值1023，范围0~1023
	AD9959_Set_Amp(CH3, 1023); 		//设置通道1幅度控制值1023，范围0~1023

	AD9959_Set_Phase(CH0, 0);			//设置通道0相位控制值0(0度)，范围0~16383
	AD9959_Set_Phase(CH3, 0);	//设置通道1相位控制值4096(90度)，范围0~16383

	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	//UPDATE = 1;

	// 需要上网络分析仪
	angle_direct = (float)12.0 - GetPhs();

	angle_direct = 0.0;
	while(1)
	{
		//Serial_Printf("%d/r/n",GetMag());
		Serial_Printf("%d\r\n",GetPhs());
		delay_ms(500);
	}
	// 长度已知，此处反解出来
	/*
	while (1)
	{
		if (flag == 1)
		{
			Serial_SendByte(0x11); // 使串口屏显示测量中

			Mea_Length();
			// 需要细化小数问题
			Serial_Printf("n2.val=%d", length);

			Serial_SendByte(0x22); // 使串口屏显示结果
			flag = 0;
		}
		if (flag == 2)
		{
			Serial_SendByte(0x11); // 使串口屏显示测量中

			Mea_C();
			Serial_Printf("n0.val=%d", CorR);

			if (CorR == 1)
				Serial_Printf("n1.val=%f", C);
			else if (CorR == 2)
				Serial_Printf("n1.val=%f", R);

			Serial_SendByte(0x33); // 使串口屏显示结果
			flag = 0;
		}
	}*/
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
	fre = 2.5;
	AD9959_Set_Fre(CH0, fre*1000000); // 2.5MHZ 对应波长 84m 可测量最长21m
	IO_Update();
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
		fre = 5.0;
		// AD9954_Set_Fre(fre); // 5.0MHZ 对应波长 42m 可测量最长10.5m
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
			fre = 10.0;
			// AD9954_Set_Fre(fre); // 10.0MHZ 对应波长 21m 可测量最长5.25m
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
				fre = 20.0;
				// AD9954_Set_Fre(fre); // 20.0MHZ 对应波长 10.5m 可测量最长2.625m
				for (int i = 0; i < 100; i++)
				{
					length_result[i] = Cal_Length((float)10.5);
				}
			}
		}
	}
	length = average(length_result);
}

float Cal_Length(float lambda)
{
	float angle = GetPhs() + angle_direct;
	float result = (angle * lambda) / (float)(360 * 2);
	return result;
}

void Mea_C(void)
{
	float C1, C2, C3;
	// DDS输入固定频率1信号
	fre = 60;
	// AD9954_Set_Fre(fre);
	Cal_C();
	C1 = C;

	// DDS输入固定频率2信号
	fre = 50;
	// AD9954_Set_Fre(fre);
	Cal_C();
	C2 = C;

	// DDS输入固定频率3信号
	fre = 40;
	// AD9954_Set_Fre(fre);
	Cal_C();
	C3 = C;

	// 比较
	uint8_t same_flag = compare(C1, C2, C3);

	if (same_flag)
	{
		CorR = 1;
		// 电容测量100次，队列取平均
		for (int i = 0; i < 100; i++)
		{
			Cal_C();
			C_result[i] = C;
		}
		C = average(C_result);
	}
	else
	{
		CorR = 2;
		// 电阻测量100次，队列取平均
		for (int i = 0; i < 100; i++)
		{
			R = R0 / Av;
			R_result[i] = R;
		}
		R = average(R_result);
	}
}

void Cal_C(void)
{
	de_phi = GetPhs();
	Av = GetMag();

	Cf = (float)1.44 + (float)95.27 * length;
	C = sqrt((R0 / Av) * (R0 / Av) / (float)2 * (float)pi * fre) - Cf;
}

float myabs(float a)
{
	return (a > 0) ? a : (-a);
}

uint8_t compare(float c1, float c2, float c3)
{
	uint8_t same_flag;
	float de1 = c2 - c1;
	float de2 = c3 - c2;
	if ((myabs(de1) < (float)0.01) && (myabs(de2) < (float)0.01))
	{
		same_flag = 1;
	}
	else
	{
		same_flag = 0;
	}
	return same_flag;
}

float average(float *num)
{
	float sum;
	for (int i = 0; i < 100; i++)
	{
		sum += num[i] * (float)0.01;
	}
	return sum;
}
