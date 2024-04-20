#include "stm32f4xx.h"
#include "TIM.h"
#include "GPIO.h"
#include "Serial.h"
#include "TODA.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_Config();
	Serial_Init();
	TIM_Init();
    /*Add your code here*/
	//Serial_Printf("Hello from stm%d\n",32);
	Serial_Printf("va0.val=60");
	Serial_End();
	Serial_Printf("va1.val=60");
	Serial_End();
	Serial_Printf("va2.val=1");
	Serial_End();
    while(1);
}
