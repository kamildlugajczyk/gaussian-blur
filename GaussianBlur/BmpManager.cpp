#pragma pack(push, 1)
#include "BmpManager.h"
#include <fstream>


//std::ofstream outputFileStream(outputPath, std::ios::binary | std::ios::out);

BmpManager::BmpManager(std::string inputPath, std::string outputPath)
{
	this->inputPath = inputPath;
	this->outputPath = outputPath;
}

void BmpManager::loadBitmap(unsigned char* &inputArray, unsigned char* &outputArray)
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

		inputArray = new unsigned char[bmpHeader.biWidth * bmpHeader.biHeight * 3] { 0 };
		outputArray = new unsigned char[bmpHeader.biWidth * bmpHeader.biHeight * 3] { 0 };

		padding = (4 - (bmpHeader.biWidth * 3 % 4)) % 4;

		inputFileStream.seekg(fileHeader.bfOffBits, std::ios::beg);
		for (int i = 0; i < bmpHeader.biHeight; i++)
		{
			inputFileStream.read((char*)&inputArray[i * bmpHeader.biWidth * 3], sizeof(unsigned char) * bmpHeader.biWidth * 3);
			inputFileStream.seekg(padding, std::ios_base::cur);
		}

		inputFileStream.seekg(fileHeader.bfOffBits, std::ios::beg);
		for (int i = 0; i < bmpHeader.biHeight; i++)
		{
			inputFileStream.read((char*)&outputArray[i * bmpHeader.biWidth * 3], sizeof(unsigned char) * bmpHeader.biWidth * 3);
			inputFileStream.seekg(padding, std::ios_base::cur);
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

int32_t BmpManager::getWidth()
{
	return bmpHeader.biWidth;
}

int32_t BmpManager::getHeight()
{
	return bmpHeader.biHeight;
}

#pragma pack(pop)