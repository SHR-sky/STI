#ifndef __FFT_ANALYSIS_H__
#define __FFT_ANALYSIS_H__

#include "sys.h"
#include "stm32_dsp.h"
#include "math.h"

/*
Usage:
频率 = Fs/N * MagBufIndex[i]
Fs为采样频率，N为采样点，i为第几个
Vpp = MagBufArray[i]*2/N
Pha = PhaBufArray[i]
*/

#define SAMPLE_NODE_NUM 1024 // 1024点FFT

void GetSpectrum(void);       // 获得频谱
void GetPowerMag(void);       // 计算幅度
void GetPowerMagAndPha(void); // 获取幅度与相位
void SortSpec(void);          // 对频谱从大到小进行排序
u16 GetMaxSpec(void);         // 获取最大频谱处下标

double CalcTHD(u16 n); // 计算谐波失真，取最多n个谐波

#endif
