#pragma pack(push, 1)
#include "BmpManager.h"
#include <fstream>


BmpManager::BmpManager(std::string inputPath, std::string outputPath)
{
	this->inputPath = inputPath;
	this->outputPath = outputPath;
}

void BmpManager::loadBitmap(unsigned char* &inputArray, unsigned char*& inputArrayWithFrame, unsigned char* &outputArray, char size)
{
	std::ifstream inputFileStream(inputPath, std::ios::binary);

	if (!inputFileStream.is_open())
	{
		// TODO error message
	}
	else
	{
		int boundary = size / 2;

		inputFileStream.read((char*)&fileHeader, sizeof(fileHeader));	// wczytanie naglowka pliku do struktry
		inputFileStream.read((char*)&bmpHeader, sizeof(bmpHeader));		// wczytanie naglowka bmp do struktury

		inputArray = new unsigned char[bmpHeader.biWidth * bmpHeader.biHeight * 3] { 0 };
		inputArrayWithFrame = new unsigned char[(bmpHeader.biWidth + size - 1) * (bmpHeader.biHeight + size - 1) * 3] { 0 };

		outputArray = new unsigned char[bmpHeader.biWidth * bmpHeader.biHeight * 3] { 0 };

		padding = (4 - (bmpHeader.biWidth * 3 % 4)) % 4;
		
		inputFileStream.seekg(fileHeader.bfOffBits, std::ios::beg);
		for (int i = 0; i < bmpHeader.biHeight; i++)
		{
			inputFileStream.read((char*)&inputArray[i * bmpHeader.biWidth * 3], sizeof(unsigned char) * bmpHeader.biWidth * 3);
			inputFileStream.seekg(padding, std::ios_base::cur);
		}

		for (int i = boundary; i < bmpHeader.biHeight + boundary; i++)
		{
			for (int j = boundary; j < bmpHeader.biWidth + boundary; j++)
			{
				inputArrayWithFrame[3 * (i * (bmpHeader.biWidth + size - 1) + j)] = inputArray[3 * ((i - boundary) * bmpHeader.biWidth + (j - boundary))];
				inputArrayWithFrame[3 * (i * (bmpHeader.biWidth + size - 1) + j) + 1] = inputArray[3 * ((i - boundary) * bmpHeader.biWidth + (j - boundary)) + 1];
				inputArrayWithFrame[3 * (i * (bmpHeader.biWidth + size - 1) + j) + 2] = inputArray[3 * ((i - boundary) * bmpHeader.biWidth + (j - boundary)) + 2];
			}
		}

		inputFileStream.close();
	}
}

void BmpManager::saveBitmap(unsigned char* &outputArray)
{
	std::ofstream outputFileStream(outputPath, std::ios::binary);

	if (!outputFileStream)
	{
		// TODO error message
	}
	else
	{
		outputFileStream.write((char*)&fileHeader, sizeof(fileHeader));
		outputFileStream.write((char*)&bmpHeader, sizeof(bmpHeader));
		char* padArray = new char[padding] { 0 };
		
		for (int i = 0; i < bmpHeader.biHeight; i++)
		{
			outputFileStream.write((char*)&outputArray[i * bmpHeader.biWidth * 3], sizeof(unsigned char) * bmpHeader.biWidth * 3);
			outputFileStream.write(padArray, sizeof(unsigned char) * padding);
		}
		outputFileStream.close();
	}
}

//void BmpManager::mirrorBoundaries(unsigned char*& inputArray, char size)
//{
//
//}

int32_t BmpManager::getWidth()
{
	return bmpHeader.biWidth;
}

int32_t BmpManager::getHeight()
{
	return bmpHeader.biHeight;
}

#pragma pack(pop)