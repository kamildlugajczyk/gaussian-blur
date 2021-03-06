#pragma once

#ifdef CPPLIB_EXPORTS
#define CPPLIB_API __declspec(dllexport)
#else
#define CPPLIB_API __declspec(dllimport)
#endif

extern "C" CPPLIB_API void gauss(unsigned char* inputArray, unsigned char * outputArray, float* kernel, int32_t width, int32_t startHeight, int32_t stopHeight, int32_t size, float sum);
