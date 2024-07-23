#include "sys.h"

uint8_t mode_flag = 0;



// 加电压跟随器，提高输入电阻
// 或者增益自举电路

#define Contrl1 PAout(1)
#define Contrl2 PAout(2)
#define Contrl3 PAout(3)



void Cal_V(void);
void Cal_I(void);
void Cal_R(void);

int main()
{
	
	if(mode_flag == 1)
    {
        Cal_V();
    }
    else if(mode_flag == 2)
    {
        Cal_I();
    }
	else if(mode_flag == 3)
    {
        Cal_R();    
    }

	
}


void Cal_V(void)
{
    // clear
    Contrl1 = 0;
    Contrl2 = 0;
    Contrl3 = 0;

    // open_10V
    Contrl1 = 1;
    // Vout
    double Vout = Get_Adc(ADC2,CH0);


}


