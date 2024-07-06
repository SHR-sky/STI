#include "sys.h"
#include "math.h"

#define pi 3.1415926
#define E 2.718281828

uint8_t flag = 0;

// 电缆速度系数
// 可以采用安慰按钮，此处取0.672285，或者修改为更好的值
float k;

// 反馈阻值
const float R0 = 0.001; // 1k

// 单位长度寄生电容（pF）
float Cp = 95.27;

// 单位长度电阻
float Rp = 0;

// 多次测量得结果
float length = 0;
float C = 0;
float R = 0;
float length_result[100];
float C_result[100];
float R_result[100];

// 增益
float Av;

// 相位以及相位差
float phi;
float de_phi;

// 标志是电容还是电阻
uint8_t CorR;

// 用于标定的标准波长80m，对应10m线，对应角度90度
float lambda0;

//校准函数
void direct(void);

// 波长转相位
int lambdatofre(float);

// 测长度
void Mea_Length(void);

// 测阻抗
void Mea_C(void);


uint8_t compare(float c1, float c2, float c3);
float average(float *num);
float myabs(float a);

int main(void)
{		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Serial_Init();
	Serial_Printf("Hi!");
	// 初始化
	Adc_Init();
	AD9959_Init();								//初始化控制AD9959需要用到的IO口,及寄存器
	
	AD9959_Set_Fre(CH3, 2000000);	//设置通道3频率2M
	AD9959_Set_Amp(CH3, 1023); 		//设置通道3幅度控制值1023，范围0~1023
	AD9959_Set_Phase(CH3, 0);	//设置通道3相位控制值，4096(90度)，范围0~16383

	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while (1)
	{
		if (flag == 3)
		{
			direct();
		}


		if (flag == 1)
		{
			//Serial_SendByte(0x11); // 使串口屏显示测量中

			Mea_Length();
			// 需要细化小数问题
			Serial_Printf("n2.val=%f\r\n", length);

			//Serial_SendByte(0x22); // 使串口屏显示结果
			//flag = 0;
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
	}

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

void direct()
{
	float alpha = GetPhs();
	k = ((float)20/alpha)*2.625/((float)3 * 100);
}

int lambdatofre(float lambda)
{
	int fre = k * 300000000/lambda; 
	return fre;
}


void Mea_Length(void)
{
	// 10m 应该使得 2.625MHZ频率，波长为80m，相位为90度
	// 即是，插入后，测量角度，得到为alpha相位时
	// 不为90是波长不为80m导致的，所以，需要修正波长
	// lambda等于360 * 2 * 10（d）/alpha
	// lambda又等于 光速*系数/f
	// 由此可得系数，用于修正
	// 由此使得，产生的频率确实能够得到标准lambda


	// 给80m波长
	AD9959_Set_Fre(CH3,lambdatofre(80));
	de_phi = GetPhs();
	if(de_phi>90)	// 说明大于10m
	{
		// 给40m波长
		AD9959_Set_Fre(CH3,lambadatofre(40));
		de_phi = GetPhs();
		if(de_phi>90)	// 说明大于15m
		{
			for(int i=1; i<100; i++)
			{
				AD9959_Set_Fre(CH3,lambadatofre(40 + 0.01 * i));
				if(de_phi == 90)
				{
					length = (40 + 0.01*i)/8.0;
					break;
				}
			}
		}
		else if(de_phi<90)	//说明小于15m
		{
			for(int i=1; i<100; i++)
			{
				AD9959_Set_Fre(CH3,lambadatofre(40 - 0.01 * i));
				if(de_phi == 90)
				{
					length = (40 - 0.01*i)/8.0;
					break;
				}
			}
		}
	}
	else if(phi<90)	// 小于10m
	{
		// 给160m波长
		AD9959_Set_Fre(CH3,lambadatofre(160));
		de_phi = GetPhs();
		if(de_phi>90)	// 说明大于5m
		{
			for(int i=1; i<100; i++)
			{
				AD9959_Set_Fre(CH3,lambadatofre(160 + 0.01 * i));
				if(de_phi == 90)
				{
					length = (160 + 0.01*i)/8.0;
					break;
				}
			}
		}
		else if(de_phi<90)	// 说明小于5m
		{
			for(int i=1; i<100; i++)
			{
				AD9959_Set_Fre(CH3,lambadatofre(160 - 0.01 * i));
				if(de_phi == 90)
				{
					length = (160 - 0.01*i)/8.0;
					break;
				}
			}
		}
	}
	
}



void Mea_C(void)
{

	AD9959_Set_Fre(CH3,100000);
	float C1 = GetPhs();

	AD9959_Set_Fre(CH3,200000);
	float C2 = GetPhs();

	AD9959_Set_Fre(CH3,300000);
	float C3 = GetPhs();

	uint8_t flag_c = (C1,C2,C3);
	if(flag_c == 1)
	{
		R =  R0 / GetMag() - Rp;
	}
	else
	{
		// excel 拟合？或者NE555
		C = (float)0.0014*pow(E,0.0775*GetPhs()) - length * Cp;
	}
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
