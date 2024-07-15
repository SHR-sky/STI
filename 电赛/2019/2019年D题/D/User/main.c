#include "sys.h"

const double R_stand = 16.8;
const double RL = 1.6;
double VDC_in;

double V_stand;
double Vin;
double Vout;

double Iin;
double Iout;

double Av;

double Rin;
double Rout;

int Fre_up;
int Fre_down;

int main()
{
	Adc_Init();

	AD9959_Init();
	AD9959_Set_Fre(CH0,1000); //写频率
	AD9959_Set_Amp(CH0,1023);//写幅度
	AD9959_Set_Phase(CH0,0);//写相位
	
	IO_Update(); 

}



double get_Vpp(ADC_TypeDef* kADCx,u8 ch)
{
	double Vmax;
	double Vmin;
	double V;

	for(int i=0; i<1000; i++)
	{
		V = Get_Adc(kADCx,ch);
		if(V>Vmax)
			Vmax = V;
		else if(V<Vmin)
			Vmin = V;
	}
	return Vmax - Vmin;
}

void Open_cir(void)
{
	// 继电器开关打开
}

void Close_cir(void)
{
	// 继电器开关闭合
}

void Cal_Rin(void)
{
	// 非常低效的峰峰值检测

	V_stand = get_Vpp(ADC1, CH1);
	Vin = get_Vpp(ADC2, CH1);
	
	Rin = Vin * R_stand / (V_stand-Vin);
}

void Cal_Rout(void)
{
	Open_cir();
	double Voff = Get_Adc(ADC1,CH2);
	Close_cir();
	double Von = Get_Adc(ADC1,CH2);
	Rout = (Voff - Von) * RL / Von;
}

void Cal_Av(void)
{
	// AD FFT
	// 快速傅里叶之后，再去求得第一谐波的幅度



	// 检波器
	// 检出为真有效值，再去运算
}

void Cal_Fre_up(void)
{
	for(int i=1000;; i++)
	{
		AD9959_Set_Fre(CH0,i);
		IO_Update();

		if(Av < 0.7071)
		{
			Fre_up = i;
			break;
		}
	}
}

void Cal_curve(void)
{
	for(int i=1;i<10000000; i++)
	{
		AD9959_Set_Fre(CH0,i);
		IO_Update();
		Cal_Av();
		Serial_Printf("va1.val=%d",Av);
		Serial_Printf("va2.val=%d",100*(Av-(int)Av));
	}
}


void judge_cir(void)
{

}






