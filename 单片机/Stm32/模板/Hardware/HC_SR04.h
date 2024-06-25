#ifndef _HC_SR04_H
#define _HC_SR04_H    
#include "sys.h"

#define TRIG_Send  PBout(11)
#define ECHO_Reci  PBin(10)
 
void HC_Init(void);
static void OpenTimerForHc(void);
static void CloseTimerForHc(void);
void TIM4_IRQHandler(void);
uint32_t GetEchoTimer(void);
float HC_Get(void );

#endif
