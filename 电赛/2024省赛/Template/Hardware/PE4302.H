#ifndef _PE4302_H_
#define _PE4302_H_

#include "sys.h"
#include "delay.h"

#define PE_LE_0					PCout(9)
#define PE_CLK					PCout(10)
#define PE_DAT					PCout(11)

#define PE_LE_0_EN      PE_LE_0 = 1
#define PE_LE_0_DIS     PE_LE_0 = 0

#define PE_CLK_0        PE_CLK = 0
#define PE_CLK_1        PE_CLK = 1

#define PE_DAT_0        PE_DAT = 0
#define PE_DAT_1        PE_DAT = 1

void PE_GPIO_Init(void);
void PE4302_0_Set(unsigned char db);
void PE4302_1_Set(unsigned char db);
#endif
