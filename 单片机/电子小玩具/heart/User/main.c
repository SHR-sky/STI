#include "stm32f4xx.h"                  // Device header
#include "math.h"
#include "heart.h"	


void delay_ms(uint16_t time);

int main()
{   
	uint8_t t = 0;
	double Fx1,Fx2,Fx3,Fx4,x = 1.8165;
	Dac1_Init();
	while(1)
	{
		Fx1 = pow(x,0.6666);
		Fx2 = sqrt(3.3 - pow(x,2));
		Fx3 = sin(31.41592 * x);
		Fx4 = (Fx1 + 0.9 * Fx2 * Fx3) * 900;
		
		Dac1_Set_Vol(Fx4 + 1100);//DAC
		
		if(t == 0)
		{
			x = x - 0.001;
		}
		else
		{
			x = x + 0.001;
		}
		if(x <= 0) 
			t = 1;
		if(x == 1.8165)
		{
			t = 0;
			delay_ms(1000);
		}
	}
}

void delay_ms(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=12000;
      while(i--) ;    
   }
}

