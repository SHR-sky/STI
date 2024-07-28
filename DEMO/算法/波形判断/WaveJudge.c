#include "WaveJudge.h"

// AUTHOR: Rufish 2024.07.22
// REFERENCES: https://zhuanlan.zhihu.com/p/496245337

short AD_Judge_Value[SAMPLE_LENGTH];
short diffSampleWave[SAMPLE_LENGTH - 1];

u8 AppoximateWaveJudge(void)
{
    int cnt = 0;
    double percent;
    for (int i = 0; i < SAMPLE_LENGTH; i++)
    {
        if (AD_Judge_Value[i] >= COMPARE_VALUE)
        {
            cnt += 1;
        }
    }
    percent = (double)cnt / (double)SAMPLE_LENGTH;
    if (percent > PERCENTAGE_RECT)
    {
        return RECT_WAVE;
    }
    else if (percent > PERCENTAGE_SIN)
    {
        return SIN_WAVE;
    }
    else if (percent > PERCENTAGE_TRI)
    {
        return TRI_WAVE;
    }
    else
    {
        return NONE_OF_ABOVE;
    }
}

u8 WaveJudge(void)
{
    int mmax = 0;
    u8 isInc = 0;
    int indexFirst, indexSecond;
    int mmin;
    for (int i = 0; i < SAMPLE_LENGTH - 1; i++) // 找第一个（位于最后的）最大的
    {
        diffSampleWave[i] = AD_Judge_Value[i + 1] - AD_Judge_Value[i];
        if (isInc == 1 && diffSampleWave[i] < 0) // 前一次递增，这次递减，说明前一个数最大
        {
            isInc = 2;
            mmax = AD_Judge_Value[i];
            indexFirst = i;
            break;
        }
        if (diffSampleWave[i] >= 0)
        {
            isInc = 1; // 持续增长
        }
    }
    isInc = 0; // 继续寻找
    for (int i = indexFirst + 1; i < SAMPLE_LENGTH - 1; i++)
    {
        diffSampleWave[i] = AD_Judge_Value[i + 1] - AD_Judge_Value[i];
        if (isInc == 1 && diffSampleWave[i] < 0) // 前一次递增，这次递减，说明前一个数最大
        {
            isInc = 2;
            mmax = AD_Judge_Value[i];
            indexSecond = i;
            break;
        }
        if (diffSampleWave[i] >= 0)
        {
            isInc = 1; // 持续增长
        }
    }

    mmin = AD_Judge_Value[(indexFirst + indexSecond) / 2]; // 最小值

    indexSecond -= 1; // 退后一个点，保持First-Second有一个周期
    quickSort(diffSampleWave, indexFirst, indexSecond);
    // 高斯滤波
    indexFirst += 1;  // 去掉最小值
    indexSecond -= 1; // 去掉最大值

    int sum = 0;
    for (int i = indexFirst; i <= indexSecond; i++)
    {
        sum += diffSampleWave[i];
    }
    if (sum > (mmax - mmin) / 8) // 避免采样误差，大于10，无误差应该为0
    {
        return SAWTOOTH_WAVE;
    }
    else
    {
        sum = 0;
        for (int i = indexFirst; i <= indexSecond; i++)
        {
            sum += myAbs(diffSampleWave[i]);
        }
        if (sum < 10) // 避免采样误差，大于10，无误差应该为0
        {
            return RECT_WAVE;
        }
        else // 正弦与三角
        {
            indexFirst -= 1;                                                // 复原
            indexSecond += 1;                                               // 复原
            double dot_3_4 = mmin * 1.0 + (mmax * 1.0 - mmin * 1.0) * 0.75; // 3/4处
            int cnt = 0;
            for (int i = indexFirst; i <= indexSecond; i++)
            {
                if (AD_Judge_Value[i] > dot_3_4)
                    cnt += 1;
            }
            if (cnt * 1.0 / ((indexSecond - indexFirst + 1) * 1.0) > 0.26) // 正弦
            {
                return SIN_WAVE;
            }
            else if (cnt * 1.0 / ((indexSecond - indexFirst + 1) * 1.0) > 0.20) // 三角
            {
                return TRI_WAVE;
            }
            else
            {
                return NONE_OF_ABOVE;
            }
        }
    }
}

void swap(short *a, short *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(short arr[], int low, int high)
{
    // 创建一个栈
    short stack[high - low + 1];
    int top = -1; // 初始化栈顶

    // 将初始的 low 和 high 压入栈
    stack[++top] = low;
    stack[++top] = high;

    // 循环处理栈中的元素
    while (top >= 0)
    {
        // 出栈 high 和 low
        high = stack[top--];
        low = stack[top--];

        // 划分数组，找到基准值的位置
        int pivot = arr[high];
        int i = low - 1; // 小于基准值的区间边界

        for (int j = low; j <= high - 1; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }

        swap(&arr[i + 1], &arr[high]);
        int pivot_pos = i + 1;

        // 将新的子数组压入栈
        if (pivot_pos - 1 > low)
        {
            stack[++top] = low;
            stack[++top] = pivot_pos - 1;
        }
        if (pivot_pos + 1 < high)
        {
            stack[++top] = pivot_pos + 1;
            stack[++top] = high;
        }
    }
}

short myAbs(short a)
{
    return a < 0 ? -a : a;
}