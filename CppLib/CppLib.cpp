#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <iostream>
#include <limits.h>
#include "CppLib.h"

void gauss(unsigned char * inputArray, unsigned char* outputArray, float* kernel, int32_t width, int32_t startHeight, int32_t stopHeight, int32_t size, float sum)
{

    int boundary = size / 2;
    int inputIndex = 0, outputIndex = 0, kernelIndex = 0;
    float sumG = 0.0, sumB = 0.0, sumR = 0.0;

    for (int i = boundary + startHeight; i < stopHeight + boundary; i++)
    {
        for (int j = boundary; j < width + boundary; j++)
        {
            sumG = 0.0, sumB = 0.0, sumR = 0.0;

            for (int k = -boundary; k <= boundary; k++)
            {
                for (int l = -boundary; l <= boundary; l++)
                {
                    inputIndex = 3 * ((i + k) * (width + size - 1) + (j + l));
                    kernelIndex = (k + boundary) * size + l + boundary;

                    sumB += kernel[kernelIndex] * inputArray[inputIndex];
                    sumG += kernel[kernelIndex] * inputArray[inputIndex + 1];
                    sumR += kernel[kernelIndex] * inputArray[inputIndex + 2];
                }
            }

            outputIndex = 3 * ((i - boundary) * width + (j - boundary));

            outputArray[outputIndex] = sumB / sum;               // B
            outputArray[outputIndex + 1] = sumG / sum;           // G
            outputArray[outputIndex + 2] = sumR / sum;           // R
        }
    }
}