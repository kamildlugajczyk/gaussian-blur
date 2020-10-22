#pragma pack(push, 1)
#include "BmpManager.h"
#include <fstream>


//std::ofstream outputFileStream(outputPath, std::ios::binary | std::ios::out);

BmpManager::BmpManager(std::string inputPath, std::string outputPath)
{
	this->inputPath = inputPath;
	this->outputPath = outputPath;
}

void BmpManager::loadBitmap()
{
	std::ifstream inputFileStream(inputPath, std::ios::binary);

	if (!inputFileStream.is_open())
	{
		// TODO error message
	}
	else
	{
		inputFileStream.read((char*)&fileHeader, sizeof(fileHeader));	// wczytanie naglowka pliku do struktry
		inputFileStream.read((char*)&bmpHeader, sizeof(bmpHeader));		// wczytanie naglowka bmp do struktury

		bmpArray = new uint8_t[bmpHeader.biWidth * bmpHeader.biHeight * 3];

		inputFileStream.seekg(fileHeader.bfOffBits, std::ios::beg);

		padding = (4 - (bmpHeader.biWidth * 3 % 4)) % 4;
		loadPixels(inputFileStream);									// wczytanie kolorow

		inputFileStream.close();
	}
}

void BmpManager::saveBitmap()
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
			outputFileStream.write((char*)&bmpArray[i * bmpHeader.biWidth * 3], sizeof(uint8_t) * bmpHeader.biWidth * 3);
			outputFileStream.write(padArray, sizeof(uint8_t) * padding);
		}
		outputFileStream.close();
	}
}

void BmpManager::loadPixels(std::ifstream & inputFileStream)
{
	for (int i = 0; i < bmpHeader.biHeight; i++)
	{
		inputFileStream.read((char*)&bmpArray[i * bmpHeader.biWidth * 3], sizeof(uint8_t) * bmpHeader.biWidth * 3);
		inputFileStream.seekg(padding, std::ios_base::cur);
	}
}

#pragma pack(pop)