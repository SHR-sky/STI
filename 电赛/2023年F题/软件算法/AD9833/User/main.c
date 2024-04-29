#include "stm32f4xx.h"
#include "AD9833.h"
#include "sys_tick.h"


int main()
{
    /*Add your code here*/
	AD9833InitConfig();
	AD9833_WaveSeting(10000, 0, SIN_WAVE, 0);
	while(1)
	{
		// ɨƵ
		for(int i=15; i<=20; i++)
		{
			AD9833_WaveSeting(i*1000.0, 0, SIN_WAVE, 0);
			//Delay_ms(10000);
			Delay_s(1);
		}
		
	}
	//AD9833_WaveSeting(10000, 0, SIN_WAVE, 0);
	//GPIO_SetBits(GPIOB, GPIO_Pin_14);
	//for(int i=0; i<100000; i++);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	//for(int i=0; i<100000; i++);
	//while(1);
}
