#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <iostream>
#include <limits.h>
#include "CppLib.h"

void gauss(unsigned char * inputArray, unsigned char* outputArray, double kernel[5][5], int32_t width, int32_t height, char size, double & sum)
{
    sum += kernel[0][0];
    sum += inputArray[0];
    // operator trojargumetnowey 
    int border = 0;
    size == 3 ? border = 1 : border = 2;          // gdy 3x3 to w pteli odejmuje 1; else 2

    //for (int i = border; i < (height - border) * 3; i++)
    //{
    //    for (int j = border; j < (width - border) * 3; j++)
    //    {
    //        double sumG = 0.0, sumB = 0.0, sumR = 0.0;
    //        for (int k = -border; k <= border; k++)
    //        {
    //            for (int l = -border; l <= border; l++)
    //            {
    //                sumB += kernel[k + border][l + border] * inputArray[(i + k) * width + (j + l)];
    //                /*sumG += kernel[k + border][l + border] * inputArray[(i + k) * width + (j + l) + 1];
    //                sumR += kernel[k + border][l + border] * inputArray[(i + k) * width + (j + l) + 2];*/
    //            }
    //        }
    //        outputArray[(i * width + j)] = sumB / sum;               // B
    //        outputArray[(i * width + j) + 1] = sumG / sum;           // G
    //        outputArray[(i * width + j) + 2] = sumR / sum;           // R
    //    }
    //}
}