#ifndef __task_manage_H
#define __task_manage_H
#include "stm32f10x.h"
#include "lcd.h"
#include "key.h"

#define Interface 2//界面总数
#define StrMax    10//缓存数据大小

extern u32 SysTimer;

void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn);
void Set_PointFre(u32 Key_Value, u8* Task_ID);

void Task0_PointFre(u32 Key_Value);
void Task3_SweepFre(u32 Key_Value);
void fre_buf_change(u8 *strbuf);

#endif
