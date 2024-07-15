#include "sys.h"


const double R_stand = 100.2;
const double VDC_in = 3.103;

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
	
	float EcgDiscrList[28];
	float stdResultDiffECG;

	arm_std_f32(EcgDiscrList,28-4,&stdResultDiffECG);
	
	Adc_Init();

	AD9959_Init();
	AD9959_Set_Fre(CH0,1000); //写频率
	AD9959_Set_Amp(CH0,1023);//写幅度
	AD9959_Set_Phase(CH0,0);//写相位
	
	IO_Update(); 


}



int a[111];
int b[222];


void Cal_Rin(void)
{
	// 非常低效的峰峰值检测

	Vin = Get_Adc_Average(ADC1, CH1, 100) * 3.3  / 4096.0 ;
	V_stand = Get_Adc_Average(ADC1, CH2, 100) * 3.3 / 4096.0 ;

	Iin = ( (V_stand- VDC_in) - (Vin- VDC_in) ) / R_stand;

	Rin = 0.5 / Iin;
}

void Cal_Rout(void)
{

}

void Cal_Av(void)
{

}

void Cal_Curve(void)
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







