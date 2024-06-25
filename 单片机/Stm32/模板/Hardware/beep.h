#ifndef __BEEP_H
#define __BEEP_H     
#include "sys.h"

// 无源蜂鸣器，用于提示音

#define BEEP PAout(11)  

void BEEP_Init(void);         
void Sound(uint16_t frq);
void Sound2(uint16_t time);
void play_music(void);
void play_successful(void);
void play_failed(void);

#endif
