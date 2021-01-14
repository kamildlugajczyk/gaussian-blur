#pragma once
#include <iostream>

typedef struct {
	uint16_t  bfType;		//2
	uint32_t bfSize;		//4
	uint16_t  bfReserved1;	//2
	uint16_t  bfReserved2;	//2
	uint32_t bfOffBits;		//4
} BMPFILEHEADER;

typedef struct {
	uint32_t biSize;		//4
	int32_t  biWidth;		//4
	int32_t  biHeight;		//4
	uint16_t  biPlanes;
	uint16_t  biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BMPINFOHEADER;

typedef struct {
	uint32_t        bV5Size;
	int32_t         bV5Width;
	int32_t         bV5Height;
	uint16_t         bV5Planes;
	uint16_t         bV5BitCount;
	uint32_t        bV5Compression;
	uint32_t        bV5SizeImage;
	int32_t         bV5XPelsPerMeter;
	int32_t         bV5YPelsPerMeter;
	uint32_t        bV5ClrUsed;
	uint32_t        bV5ClrImportant;
	uint32_t        bV5RedMask;
	uint32_t        bV5GreenMask;
	uint32_t        bV5BlueMask;
	uint32_t        bV5AlphaMask;
	uint32_t        bV5CSType;
	//CIEXYZTRIPLE bV5Endpoints;		-36bit
	uint32_t        bV5GammaRed;
	uint32_t        bV5GammaGreen;
	uint32_t        bV5GammaBlue;
	uint32_t        bV5Intent;
	uint32_t        bV5ProfileData;
	uint32_t        bV5ProfileSize;
	uint32_t        bV5Reserved;
} BMPV5HEADER;


class BmpManager
{
	std::string inputPath;
	std::string outputPath;
	BMPFILEHEADER fileHeader;
	BMPINFOHEADER bmpHeader;
	BMPV5HEADER bmpV5Header;
	uint8_t padding;
	int32_t width, height;

public:
	

	BmpManager(std::string inputPath, std::string outputPath);
	void loadBitmap(unsigned char* &inputArray, unsigned char*& inputArrayWithFrame, unsigned char* &outputArray, char size);
	void saveBitmap(unsigned char* &outputArray);
	void mirrorBoundaries(unsigned char*& inputArray, char size);


	int32_t getWidth();
	int32_t getHeight();
};