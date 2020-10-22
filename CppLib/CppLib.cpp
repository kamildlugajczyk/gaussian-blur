#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <iostream>
#include <limits.h>
#include "CppLib.h"

void gauss(unsigned char * inputArray, unsigned char* outputArray, double kernel[5][5], int32_t width, int32_t height, char size, double & sum)
{
    int border = 0;
    size == 3 ? border = 1 : border = 2;          // gdy 3x3 to w pteli odejmuje 1; else 2

    for (int i = border; i < (height - border) * 3; i++)
    {
        for (int j = border; j < (width - border) * 3; j++)
        {
            double sumG = 0.0, sumB = 0.0, sumR = 0.0;
            for (int k = -border; k <= border; k++)
            {
                for (int l = -border; l <= border; l++)
                {
                    sumB += kernel[k + border][l + border] * inputArray[(i + k) * width + (j + l)];
                    sumG += kernel[k + border][l + border] * inputArray[(i + k) * width + (j + l) + 1];
                    sumR += kernel[k + border][l + border] * inputArray[(i + k) * width + (j + l) + 2];
                }
            }
            
            int b = outputArray[(i * width + j)];
            outputArray[(i * width + j)] = (char)(sumB / sum) % 256;               // B
            int b1 = outputArray[(i * width + j)];

            int g = outputArray[(i * width + j) + 1];
            outputArray[(i * width + j) + 1] = (char)(sumG / sum) % 256;           // G
            int g1 = outputArray[(i * width + j) + 1];

            int r = outputArray[(i * width + j) + 2];
            outputArray[(i * width + j) + 2] = (char)(sumR / sum) % 256;           // R
            int r1 = outputArray[(i * width + j) + 2];
            int r222 = outputArray[(i * width + j) + 2];

            b = 0;
            b1 = 0;
            g = 0;
            g1 = 0;
            r = 0;
            r1 = 0;
            r222 = 0;
        }
    }
}