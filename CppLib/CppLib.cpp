#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <iostream>
#include <limits.h>
#include "CppLib.h"

void gauss(unsigned char * inputArray, unsigned char* outputArray, double** kernel, int32_t width, int32_t startHeight, int32_t stopHeight, char size, double & sum)
{

    int boundary = size / 2;

    for (int i = boundary + startHeight; i < stopHeight + boundary; i++)
    {
        for (int j = boundary; j < width + boundary; j++)
        {
            double sumG = 0.0, sumB = 0.0, sumR = 0.0;
            for (int k = -boundary; k <= boundary; k++)
            {
                for (int l = -boundary; l <= boundary; l++)
                {
                    sumB += kernel[k + boundary][l + boundary] * inputArray[3 * ((i + k) * (width + size - 1) + (j + l))];
                    sumG += kernel[k + boundary][l + boundary] * inputArray[3 * ((i + k) * (width + size - 1)+ (j + l)) + 1];
                    sumR += kernel[k + boundary][l + boundary] * inputArray[3 * ((i + k) * (width + size - 1)+ (j  + l)) + 2];
                }
            }

            outputArray[3 * ((i - boundary) * width + (j - boundary))] = sumB / sum;               // B
            outputArray[3 * ((i - boundary) * width + (j - boundary)) + 1] = sumG / sum;           // G
            outputArray[3 * ((i - boundary) * width + (j - boundary)) + 2] = sumR / sum;           // R
        }
    }
}