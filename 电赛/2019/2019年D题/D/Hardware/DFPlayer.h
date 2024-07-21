#ifndef __DFPLAYER_H_
#define __DFPLAYER_H_
#include "sys.h"

void DFPlayer_Init(void);
void DFPlayer_Checksum(void);
void DFPlayer_PlayNext(void);
void DFPlayer_PlayPre(void);
void DFPlayer_Pause(void);
void DFPlayer_Play(void);
void DFPlayer_VolumnUp(void);
void DFPlayer_VolumnDown(void);
void DFPlayer_VolumnSet(u8 v);
void DFPlayer_Reset(void);

void DFPlayer_SendByte(u8 c);
void DFPlayer_SendString(char *String);
void DFPlayer_Printf(char *format, ...);
void DFPlayer_SendCMD(void);


#endif
