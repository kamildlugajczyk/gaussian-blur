#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <iostream>
#include <limits.h>
#include "CppLib.h"

void gauss(unsigned char * inputArray, unsigned char* outputArray, double** kernel, int32_t width, int32_t height, char size, double & sum)
{

    int boundary = size / 2;

    for (int i = boundary; i < height - boundary; i++)
    {
        for (int j = boundary; j < width - boundary; j++)
        {
            double sumG = 0.0, sumB = 0.0, sumR = 0.0;
            for (int k = -boundary; k <= boundary; k++)
            {
                for (int l = -boundary; l <= boundary; l++)
                {
                    sumB += kernel[k + boundary][l + boundary] * inputArray[3 * ((i + k) * width + (j + l))];
                    sumG += kernel[k + boundary][l + boundary] * inputArray[3 * ((i + k) * width + (j + l)) + 1];
                    sumR += kernel[k + boundary][l + boundary] * inputArray[3 * ((i + k) * width + (j + l)) + 2];
                }
            }
            outputArray[3 * (i * width + j)] = sumB / sum;               // B
            outputArray[3 * (i * width + j) + 1] = sumG / sum;           // G
            outputArray[3 * (i * width + j) + 2] = sumR / sum;           // R
        }
    }
}