#ifndef __VOFA_H_
#define __VOFA_H_

#include "sys.h"

#define VOFA_SendArray Serial_SendArray // 按需修改

void JustFloat_Send(float *num, u8 channelNum);

#endif
