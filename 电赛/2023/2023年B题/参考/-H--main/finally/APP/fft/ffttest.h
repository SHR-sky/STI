#ifndef __FFTTEST_H
#define __FFTTEST_H
#include "system.h"
#include "arm_math.h"

void Create_data2handle(float32_t *p);
float CalXiebo(float32_t *wave);
float CalShuchu1(float32_t *wave); 
float CalShuchu2(float32_t *wave);
float32_t CalPhase_m(float32_t *phase);
void CalPhase(float32_t *phase,char phase_name);
float Calphase_low(float *data,uint8_t indexa);
void FFTtest(void);
void Cal_WaveAPhase(float *wave,float *wa,float *wb,float *wc,u16 size);

#endif
