#ifndef __DIGITAL_ORTH_DECOMPRESSION_H__
#define __DIGITAL_ORTH_DECOMPRESSION_H__

#include "sys.h"
#include "math.h"

#define CALC_SAMPLE_LIST ADCConvertedValue2 // 采样点

double modulus(double I, double Q);            // 计算模长
void cal_2frqs(void);            // 计算参与叠加的两个正弦信号的频率
void get_pos_angle(void);                      // 计算相位差
double corr1000_200(short *data, short *mask); // 确定频率用

#endif
