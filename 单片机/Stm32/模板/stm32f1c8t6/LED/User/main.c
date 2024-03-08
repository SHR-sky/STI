#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "tft_1.96_spi_240x240.h"
#include "Timer.h"
#include "PWM.h"
#include "IC.h"
#include "AD.h"
#include "Serial.h"
#include <string.h>

uint16_t number = 3;
uint16_t ADValue = 0;
uint8_t RxData;

/*
int main()
{

	Serial_Init();
	LED_Init();

	while(1)
	{
		if(Serial_RxFlag == 1)
		{
			if(strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED_ON();
				Serial_Printf("LED_ON_OK\r\n");
			}
			else if(strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED_OFF();
				Serial_Printf("LED_OFF_OK\r\n");
			}

			Serial_RxFlag =0;
		}
	}
	
}


*/













/*
int main()
{
	AD_Init();
	
	while(1)
	{
		ADValue = AD_GetValue();
	}
	
}


*/




int main(void)
{



	Serial_Init();


	while(1)
	{

				Serial_Printf("LED_ON_OK\r\n");

	}
	
}
	
	
	//PWM_Init();
	//PWM_SetCompare1(500);
	//PWM_SetPrescaler(720-1);

	/*
	IC_GetFreq();
	IC_GetDuty();
	*/



