#pragma once
#include <iostream>
#include <Windows.h>

class BmpManager
{
	std::string inputPath;
	std::string outputPath;

	uint8_t padding;
	int32_t width, height;
	bool v5Header;

	BITMAPV5HEADER bmpV5Header;
	BITMAPINFOHEADER bmpHeader;
	BITMAPFILEHEADER fileHeader;

public:
	BmpManager(std::string inputPath, std::string outputPath);
	int loadBitmap(unsigned char* &inputArray, unsigned char*& inputArrayWithFrame, unsigned char* &outputArray, char size);
	int saveBitmap(unsigned char* &outputArray);
	void makeHistogram(unsigned char* array, uint64_t*& R, uint64_t*& G, uint64_t*& B, int32_t width, int32_t startHeight, int32_t stopHeight);

	int32_t getWidth();
	int32_t getHeight();
};