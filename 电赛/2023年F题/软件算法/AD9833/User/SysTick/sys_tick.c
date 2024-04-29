#include "sys_tick.h"

// 1us ʱ��
void SysTick_Delay_Us( __IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	for (i=0; i<us; i++) {
		// ����������ֵ��С�� 0��ʱ��CRTL�Ĵ�����λ 16���� 1
		while ( !((SysTick->CTRL)&(1<<16)) );
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}


void Delay_ms(uint32_t msTime)
{
	for(int i=0; i<msTime; i++)
		SysTick_Delay_Us(1000);
}

void Delay_s(uint32_t sTime)
{
	for(int i=0; i<sTime; i++)
		Delay_ms(1000);
}
