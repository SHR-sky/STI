#ifndef _AD9833_H_
#define _AD9833_H_

#include "stm32f4xx.h"

#define TRI_WAVE 	0  		//输出三角波
#define SIN_WAVE 	1		//输出正弦波
#define SQU_WAVE 	2		//输出方波

void AD9833InitConfig(void);
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_AmpSet(unsigned char amp);

#endif
