#include "sys.h"

uint8_t mode_flag = 0;

// 加电压跟随器，提高输入电阻
// 或者增益自举电路

#define Contrl1 PAout(1)
#define Contrl2 PAout(2)
#define Contrl3 PAout(3)

double AV = 0;

// 千欧
#define R1 1    // 电阻测量
#define R2 60   // 电阻测量
#define R3 10   // 电压测量
#define R4 1    // 
#define R5 10
#define R6 100

#define Rs 10

double V_real = 0;
double I_real = 0;
double Rx = 0;

void Cal_V(void);
void Cal_I(void);
void Cal_R(void);

int main()
{

    if (mode_flag == 1)
    {
        Cal_V();
    }
    else if (mode_flag == 2)
    {
        Cal_I();
    }
    else if (mode_flag == 3)
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
    double Vout_10 = Get_Adc_Average(ADC1, CH0, 100);
    if (Vout_10 > 1.1)
    {
        V_real = (Vout_10 - 1) / (1 + R4 / R3);
    }
    else
    {
        // clear
        Contrl1 = 0;
        Contrl2 = 0;
        Contrl3 = 0;

        // open_1V
        Contrl2 = 1;
        // Vout
        double Vout_1 = Get_Adc_Average(ADC1, CH0, 100);
        if (Vout_1 > 1.1)
        {
            V_real = (Vout_1 - 1) / (1 + R4 / R3);
        }
        else
        {
            // clear
            Contrl1 = 0;
            Contrl2 = 0;
            Contrl3 = 0;

            // open_1V
            Contrl3 = 1;
            // Vout
            double Vout_01 = Get_Adc_Average(ADC1, CH0, 100);
            V_real = (Vout_01 - 1) / (1 + R4 / R3);
        }
    }
}

void Cal_I(void)
{
    double Vout = Get_Adc_Average(ADC2, CH0, 100);
    // mA
    I_real = (Vout - 3.3/2 ) * 1 / 0.0003 * 10
}

void Cal_R(void)
{
    double Vout = Get_Adc_Average(ADC3, CH0, 100);
    // Uref为3.3V
    Rx = (3.3 * R2) / (Vout * R1) - Rs;
}
