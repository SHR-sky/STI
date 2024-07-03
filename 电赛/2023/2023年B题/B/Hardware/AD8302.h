#ifndef __AD8302_H
#define __AD8302_H

#include "sys.h"
#include "math.h"

float GetMag(void);
float GetPhs(void);
float GetRe(float Mag,float Phs);
float GetIm(float Mag,float Phs);

#endif
