#include "sys.h"

extern uint16_t meDA1_Value[DA1_Value_Length];

int main()
{
	Serial_Init();
	Serial_Printf("Init Finish!\r\n");

	AD9959_Init();
	AD9959_Set_Fre(CH0,35000000);
	AD9959_Set_Amp(CH0,1000);
	AD9959_Set_Phase(CH0,0);
	AD9959_Set_Fre(CH1,35000000);
	AD9959_Set_Amp(CH1,1000);
	AD9959_Set_Phase(CH1,00); 
	IO_Update();
	for(int i=0;i<10; i++)
	{
		delay_s(1);
		AD9959_Set_Phase(CH1,i*1000);
		IO_Update();
	}
	//delay_s(1);
	//AD9959_Set_Phase(CH1,2000);
	
	/*
	for(int i=0; i<DA1_Value_Length; i++)
	{
		meDA1_Value[i] = 2000;
	}
	DA1_Init();
	TIM4_Init();
	TIM_Cmd(TIM4, ENABLE);
	DA2_Init();
	TIM6_Init();
	TIM_Cmd(TIM6, ENABLE);

	while(1)
	{
		delay_s(1);
		Serial_Printf("Circle:1 \r\n");
	}*/
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2,ENABLE);
	}
}
*/

