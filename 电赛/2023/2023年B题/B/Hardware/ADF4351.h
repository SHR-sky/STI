#ifndef __ADF4351_H
#define __ADF4351_H

#include "sys.h"

#define ADF4351_CLK PBout(8)
#define ADF4351_OUTPUT_DATA PBout(9)
#define ADF4351_LE PBout(10)
#define ADF4351_CE PBout(11)

#define ADF4351_INPUT_DATA PBin(9)


void ADF4351Init(void); 
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xxx.x) M Hz

#endif
