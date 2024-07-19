#include "sys.h"

/*
1为AM 2为FM 3为2ASK 4为2FSK 5为PSK 6为CW
*/
uint8_t mode_flag;
int ADCValue[1000];
uint8_t flag_start = 0;

uint8_t is_DC_flag;
uint8_t is_sin_flag;
uint8_t is_square_flag;


// 需要调参，设置计数的阈值电压
const int thre_V = 100;

int main()
{
	AD9959_Init();
	AD9959_Set_Fre(CH0,2000000);
	IO_Update();

	while(1)
	{
		if(flag_start == 1)
		{	
			judge();
			flag_start = 0;
		}
	}
}

// 第一路导通
void Cir_1(void)
{
	// 根据具体电路改动，一定记得，开之前先关
	PAout(3) = 0;
	PAout(4) = 0;
	PAout(5) = 0;

	PAout(3) = 1;
	
}

// 第二路导通
void Cir2_2(void)
{
	// 根据具体电路改动
	PAout(3) = 0;
	PAout(4) = 0;
	PAout(5) = 0;

	PAout(4) = 1;
}


// 第三路导通
void Cir_3(void)
{
	// 根据具体电路改动
	PAout(3) = 0;
	PAout(4) = 0;
	PAout(5) = 0;

	PAout(5) = 1;
}


/*比较低级的算法*/
uint8_t is_DC1(void)
{
	int first = ADCValue[0];
	int flag = 0;
	for(int i; i<999; i++)
	{
		flag += ( (ADCValue[i+1] - ADCValue[i]) )< 50 ? 0 : 1;	// 需要实际尝试调参
	}

	if(flag)
		is_DC_flag = 0;
	else
		is_DC_flag = 1;

	return is_DC_flag;
}

/*有点巧妙的方法，判断一定时间内，采样到电压值大于阈值的比例，从而判断波形*/

void compare_V(void)
{
	int cnt;
	double bigger;
	for(int i; i<1000; i++)
	{
		if(ADCValue[i] > thre_V)
			cnt++;
	}

	// 计算大于阈值的比例
	bigger = (double)cnt/1000.0;

	if(bigger > 0.4 && bigger < 0.6)
	{	
		is_square_flag = 1;
		is_DC_flag = 0;
		is_sin_flag = 0;
	}
	else if(0.1 < bigger < 0.4)
	{
		is_sin_flag = 1;
		is_DC_flag = 0;
		is_square_flag = 0;
	}
	else if(bigger > 0.9)
	{
		is_DC_flag = 1;
		is_sin_flag = 0;
		is_square_flag = 0;
	}

}


uint8_t is_DC2(void)
{
	return is_DC_flag;
}

uint8_t is_sin(void)
{
	return is_sin_flag;
}

uint8_t is_square(void)
{
	return is_square_flag;
}

uint8_t is_CW(void)
{
	uint8_t flag = 0;
	Cir_1();
	compare_V();
	if(is_DC())
		flag++;
	
	Cir_2();
	compare_V();
	if(is_sin() == 0)
		flag++;
	if(is_square() == 0)
		flag++;
	
	Cir_3();
	compare_V();
	if(is_square() == 0)
		flag++;
	
	if(flag == 4)
		return 1;
	else
		return 0;
}

uint8_t is_AM(void)
{
	uint8_t flag = 0;
	Cir_1();
	compare_V();
	if(is_sin())
		flag++;
	
	Cir_2();
	compare_V();
	if(is_sin() == 0)
		flag++;
	if(is_square() == 0)
		flag++;
	
	Cir_3();
	compare_V();
	if(is_square() == 0)
		flag++;
	
	if(flag == 4)
		return 1;
	else
		return 0;
}

uint8_t is_FM(void)
{
	uint8_t flag = 0;
	Cir_1();
	compare_V();
	if(is_sin() == 0)
		flag++;
	
	Cir_2();
	compare_V();
	if(is_sin())
		flag++;
	if(is_square() == 0)
		flag++;
	
	Cir_3();
	compare_V();
	if(is_square() == 0)
		flag++;
	
	if(flag == 4)
		return 1;
	else
		return 0;
}

uint8_t is_2ASK(void)
{
	uint8_t flag = 0;
	Cir_1();
	compare_V();
	if(is_square())
		flag++;
	
	Cir_2();
	compare_V();
	if(is_sin() == 0)
		flag++;
	if(is_square() == 0)
		flag++;
	
	Cir_3();
	compare_V();
	if(is_square() == 0)
		flag++;
	
	if(flag == 4)
		return 1;
	else
		return 0;
}

uint8_t is_2FSK(void)
{
	uint8_t flag = 0;
	Cir_1();
	compare_V();
	if(is_sin() == 0)
		flag++;
	
	Cir_2();
	compare_V();
	if(is_sin() == 0)
		flag++;
	if(is_square())
		flag++;
	
	Cir_3();
	compare_V();
	if(is_square() == 0)
		flag++;
	
	if(flag == 4)
		return 1;
	else
		return 0;
}

uint8_t is_2PSK(void)
{
	uint8_t flag = 0;
	Cir_1();
	compare_V();
	if(is_sin() == 0)
		flag++;
	
	Cir_2();
	compare_V();
	if(is_sin() == 0)
		flag++;
	if(is_square() == 0)
		flag++;
	
	Cir_3();
	compare_V();
	if(is_square())
		flag++;
	
	if(flag == 4)
		return 1;
	else
		return 0;
}

void judge(void)
{
	if(is_CW())
	{
		mode_flag = 6;
	}
	else if(is_AM())
	{
		mode_flag = 1;
	}
	else if(is_FM())
	{
		mode_flag = 2;
	}
	else if(is_2ASK())
	{
		mode_flag = 3;
	}
	else if(is_2FSK())
	{
		mode_flag = 4;
	}
	else if(is_2PSK())
	{
		mode_flag = 5;
	}

	Serial_SendByte(mode_flag);
}

