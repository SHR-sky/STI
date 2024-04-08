#include "stm32f4xx.h"
#include "TIM.h"
#include "GPIO.h"
#include "Serial.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_Config();
	Serial_Init();
	TIM_Init();
    /*Add your code here*/
	Serial_Printf("Hello from stm%d\n",32);
    while(1);
}
