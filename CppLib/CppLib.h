#pragma once

#ifdef CPPLIB_EXPORTS
#define CPPLIB_API __declspec(dllexport)
#else
#define CPPLIB_API __declspec(dllimport)
#endif


extern "C" CPPLIB_API void gauss(unsigned char* inputArray, unsigned char * outputArray, double kernel[15][15], int32_t width, int32_t height, char size, double& sum);

