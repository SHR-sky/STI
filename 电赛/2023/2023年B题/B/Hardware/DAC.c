#include "DAC.h"

#define DAC0 PDout(0)
#define DAC1 PDout(1)
#define DAC2 PDout(2)
#define DAC3 PDout(3)
#define DAC4 PDout(4)
#define DAC5 PDout(5)
#define DAC6 PDout(6)
#define DAC7 PDout(7)
#define DAC8 PDout(8)
#define DAC9 PDout(9)
#define DAC10 PDout(10)
#define DAC11 PDout(11)
#define DAC12 PDout(12)
#define DAC13 PDout(13)
#define DACCLK PDout(14)

void DAC_Init(void)
{
    // DA PC0~PD14
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = (0x7fff << 0); // Initialize PD0~12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void Set_Volt(u16 volt)
{
    DACCLK = 1;
    for (int i = 0; i < 100; i++);
    PDout(13) = (volt & 0x2000) >> 13;
    PDout(12) = (volt & (0x2000 >> 1)) >> 12;
    PDout(11) = (volt & (0x2000 >> 2)) >> 11;
    PDout(10) = (volt & (0x2000 >> 3)) >> 10;
    PDout(9) = (volt & (0x2000 >> 4)) >> 9;
    PDout(8) = (volt & (0x2000 >> 5)) >> 8;
    PDout(7) = (volt & (0x2000 >> 6)) >> 7;
    PDout(6) = (volt & (0x2000 >> 7)) >> 6;
    PDout(5) = (volt & (0x2000 >> 8)) >> 5;
    PDout(4) = (volt & (0x2000 >> 9)) >> 4;
    PDout(3) = (volt & (0x2000 >> 10)) >> 3;
    PDout(2) = (volt & (0x2000 >> 11)) >> 2;
    PDout(1) = (volt & (0x2000 >> 12)) >> 1;
    PDout(0) = (volt & (0x2000 >> 13));
    DACCLK = 0;
    for (int i = 0; i < 100; i++);
}