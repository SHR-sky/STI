#ifndef __WAVE_JUDGE_H_
#define __WAVE_JUDGE_H_

#include "sys.h"

#define SIN_WAVE 0      // 正弦
#define TRI_WAVE 1      // 三角
#define RECT_WAVE 2     // 方波
#define SAWTOOTH_WAVE 3 // 锯齿波
#define NONE_OF_ABOVE 4

#define SAMPLE_WAVE WAVE_SAMPLE_LIST // WAVE_SAMPLE_LIST为采样数组名
#define SAMPLE_LENGTH 1000           // 采样长度，需修改

u8 WaveJudge(void);                             // 判断波形
short myAbs(short a);                           // 绝对值函数
void swap(short *a, int *b);                    // 交换
void quickSort(short arr[], int low, int high); // 快排, low high可取到

#endif