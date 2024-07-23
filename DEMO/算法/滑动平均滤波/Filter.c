#include "stdint.h"
#include "Filter.h"

Sliding_Mean_Filter Example_Filter = {{0}, 0, 0, 0};

void Sliding_Mean_Filter_Calc(Sliding_Mean_Filter *filter, double Data)
{
  
  filter->sum -= filter->Filter_Buffer[filter->id];                // 减去最旧的数
  filter->sum += Data;                                             // 加进最新的数
  filter->Filter_Buffer[filter->id] = Data;                        // 将最新的数覆盖最旧的数
  filter->average = filter->sum / Sliding_Mean_Filter_Buffer_Size; // 求均值
  if (++filter->id == Sliding_Mean_Filter_Buffer_Size)
    filter->id = 0;
}