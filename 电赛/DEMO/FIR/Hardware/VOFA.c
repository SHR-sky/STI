#include "VOFA.h"

u8 _byte[4] = {0};                      // float转化为4个字节数据
u8 _tail[4] = {0x00, 0x00, 0x80, 0x7f}; // 帧尾
unsigned long _ulf;

void JustFloat_Send(float *num, u8 channelNum) // justfloat 数据协议测试
{
    for (int i = 0; i < channelNum; i++)
    {
		_ulf = *((unsigned long*)(num+i));
		_byte[0] = (unsigned char)_ulf;
		_byte[1] = (unsigned char)(_ulf >> 8);
		_byte[2] = (unsigned char)(_ulf >> 16);
		_byte[3] = (unsigned char)(_ulf >> 24);
        VOFA_SendArray(_byte, 4);
    }
    // 向上位机发送两个通道数据

    // 发送帧尾
    VOFA_SendArray(_tail, 4); // 帧尾为 0x00 0x00 0x80 0x7f
}
