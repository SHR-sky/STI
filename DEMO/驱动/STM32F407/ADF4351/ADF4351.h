#ifndef __ADF4351_H
#define __ADF4351_H

#include "sys.h"

/**
 * @brief Driver for ADF4351
 * @details PC11 - CLK
 *          PC10 - OUT
 *          PC9 - LE
 *          PC12 - CE
 *          PC10 - INPUT
 * @author Rufish
 * @date 2024.07.27
 * @copyright Copyright (c)
 **/

#define ADF4351_CLK PCout(11)
#define ADF4351_OUTPUT_DATA PCout(10)
#define ADF4351_LE PCout(9)
#define ADF4351_CE PCout(12)

#define ADF4351_INPUT_DATA PCin(10)

void ADF4351Init(void);
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre); //	(xxx.x) M Hz

#endif
