#include "BmpManager.h"
#include <fstream>


BmpManager::BmpManager(std::string inputPath, std::string outputPath)
{
	this->inputPath = inputPath;
	this->outputPath = outputPath;
}

int BmpManager::loadBitmap(unsigned char* &inputArray, unsigned char*& inputArrayWithFrame, unsigned char* &outputArray, char size)
{
	std::ifstream inputFileStream(inputPath, std::ios::binary);

	if (!inputFileStream.is_open())
	{
		return -1;
	}
	else
	{
		int boundary = size / 2;
		int headerSize = 0;

		inputFileStream.read((char*)&fileHeader, sizeof(fileHeader));
		inputFileStream.read((char*)(&headerSize), 4);
		inputFileStream.seekg(-4, std::ios::cur);

		if (headerSize == 40)												// BMPINFOHEADER
		{
			v5Header = false;
			inputFileStream.read((char*)&bmpHeader, sizeof(bmpHeader));	

			width = bmpHeader.biWidth;
			height = bmpHeader.biHeight;
		}
		else if (headerSize == 124)											// BMPV5HEADER
		{
			v5Header = true;
			inputFileStream.read((char*)&bmpV5Header, sizeof(bmpV5Header));

			width = bmpV5Header.bV5Width;
			height = bmpV5Header.bV5Height;
		}
		else
		{
			return -2;
		}

		inputArray = new unsigned char[width * height * 3] { 0 };
		inputArrayWithFrame = new unsigned char[(width + size - 1) * (height + size - 1) * 3] { 0 };

		outputArray = new unsigned char[width * height * 3] { 0 };

		padding = (4 - (width * 3 % 4)) % 4;
		
		inputFileStream.seekg(fileHeader.bfOffBits, std::ios::beg);
		for (int i = 0; i < height; i++)
		{
			inputFileStream.read((char*)&inputArray[i * width * 3], sizeof(unsigned char) * width * 3);
			inputFileStream.seekg(padding, std::ios_base::cur);
		}

		for (int i = boundary; i < height + boundary; i++)
		{
			for (int j = boundary; j < width + boundary; j++)
			{
				inputArrayWithFrame[3 * (i * (width + size - 1) + j)] = inputArray[3 * ((i - boundary) * width + (j - boundary))];
				inputArrayWithFrame[3 * (i * (width + size - 1) + j) + 1] = inputArray[3 * ((i - boundary) * width + (j - boundary)) + 1];
				inputArrayWithFrame[3 * (i * (width + size - 1) + j) + 2] = inputArray[3 * ((i - boundary) * width + (j - boundary)) + 2];
			}
		}

		inputFileStream.close();
		return 0;
	}
}

int BmpManager::saveBitmap(unsigned char* &outputArray)
{
	std::ofstream outputFileStream(outputPath, std::ios::binary);

	if (!outputFileStream)
	{
		return -1;
	}
	else
	{
		outputFileStream.write((char*)&fileHeader, sizeof(fileHeader));

		if (v5Header)
			outputFileStream.write((char*)&bmpV5Header, sizeof(bmpV5Header));
		else
			outputFileStream.write((char*)&bmpHeader, sizeof(bmpHeader));


		char* padArray = new char[padding] { 0 };
		
		for (int i = 0; i < height; i++)
		{
			outputFileStream.write((char*)&outputArray[i * width * 3], sizeof(unsigned char) * width * 3);
			outputFileStream.write(padArray, sizeof(unsigned char) * padding);
		}
		outputFileStream.close();
		return 0;
	}
}

void BmpManager::makeHistogram(unsigned char* array, uint64_t*& R, uint64_t*& G, uint64_t*& B, int32_t width, int32_t startHeight, int32_t stopHeight)
{
	for (int i = 0; i < 256; i++)
	{
		B[i] = 0;
		G[i] = 0;
		R[i] = 0;
	}

	for (int i = startHeight; i < stopHeight; i++)
	{
		for (int j = 0; j < width; j++)
		{
			B[array[i * width + j]]++;
			G[array[i * width + j + 1]]++;
			R[array[i * width + j + 2]]++;
		}
	}
}

int32_t BmpManager::getWidth()
{
	if (v5Header)
		return bmpV5Header.bV5Width;
	else
		return bmpHeader.biWidth;
}

int32_t BmpManager::getHeight()
{
	if (v5Header)
		return bmpV5Header.bV5Height;
	else
		return bmpHeader.biHeight;
}