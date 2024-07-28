#ifndef __AD9959_Serial_H__
#define __AD9959_Serial_H__

#include "sys.h"
#include "stdarg.h"

void AD9959_Serial_Init(void);
void AD9959_Init(void);
void AD9959_Reset(void);
void AD9959_WriteFre(u8 ch, u32 fre);
void AD9959_WriteAmp(u8 ch, u32 amp);
void AD9959_SendByte(u8 c);
void AD9959_SendString(char *String);
void AD9959_Printf(char *format, ...);

#endif
