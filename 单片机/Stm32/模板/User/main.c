#include "sys.h"

uint8_t temperature, dec;


int main(void)
{
    // 初始化
    Data_Out_Init();
    Data_In_Init();
    tDHT11_start();
    
    Serial_Init();
	
    while(1);
}



void TIM2_IRQHandler(void)
{
	tDHT11_rec_data(&temperature, &dec);
    Serial_Printf("%d",temperature);
    Serial_Printf("%d",dec);
}



