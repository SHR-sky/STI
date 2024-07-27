#ifndef __AD9959_Serial_H__
#define __AD9959_Serial_H__

#include "sys.h"

/**
 * @brief Driver for AD9959
 * @details USART3_TX PD8 USART3_RX PD9
 * @author Rufish
 * @date 2024.07.27
 * @copyright Copyright (c)
 **/

void AD9959_Serial_Init(void);
void AD9959_Init(void);
void AD9959_Reset(void);
void AD9959_WriteFre(u8 ch, u32 fre);
void AD9959_WriteAmp(u8 ch, u32 amp);
void AD9959_Sweep(u8 ch, u32 Start_freq, u32 End_freq, u32 step, u32 unitTime);
void void AD9959_SendByte(u8 c);
void AD9959_SendString(char *String);
void AD9959_Printf(char *format, ...)

#endif
