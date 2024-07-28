#ifndef __WAVE_JUDGE_H_
#define __WAVE_JUDGE_H_

#include "sys.h"

#define PERCENTAGE_RECT 0.48 // 方波波比例，需手动修正
#define PERCENTAGE_TRI 0.20  // 三角波比例，需手动修正
#define PERCENTAGE_SIN 0.28  // 正弦波比例，需手动修正
#define COMPARE_VALUE 2000   // 比较值，需手动修正

#define SAMPLE_LENGTH 1000           // 采样长度，需修改

#define SIN_WAVE 0      // 正弦
#define TRI_WAVE 1      // 三角
#define RECT_WAVE 2     // 方波
#define SAWTOOTH_WAVE 3 // 锯齿波
#define NONE_OF_ABOVE 4

u8 AppoximateWaveJudge(void); // 判断波形，需手动填写PERCENTAGE_SIN和COMPARE_VALUE

u8 WaveJudge(void);                             // 判断波形，自动判断
short myAbs(short a);                           // 绝对值函数
void swap(short *a, short *b);                    // 交换
void quickSort(short arr[], int low, int high); // 快排, low high可取到

#endif
