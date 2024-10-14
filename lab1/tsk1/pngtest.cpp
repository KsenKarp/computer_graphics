// pngtest.cpp : Defines the entry point for the console application.
//


#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void GrayscaleCircle(unsigned char* pIn
	         , unsigned char* pMask
			 , size_t nWidth
			 , size_t nHeight
			 , const char* OutputFileName
);

unsigned char* Create_circle_mask(unsigned char* pIn
	, size_t nWidth
	, size_t nHeight
	, const char* OutputFileName
);

int main(int argc, char* argv[])
{
	class CBitsPtrGuard
	{
	public:
		CBitsPtrGuard(unsigned char** pB) : m_ppBits(pB) { }
		~CBitsPtrGuard() { if (*m_ppBits) delete *m_ppBits, *m_ppBits = 0; }
	protected:
		unsigned char** m_ppBits;
	};

	// parse input parameters
	char	szInputFileName[256];
	char    szOutputFileName[256];
	if (argc < 2)
		printf("\nformat: pngtest <input_file> [<output_file>]");
	else
	{
		strcpy(szInputFileName, argv[1]);
		if (argc > 2)
			strcpy(szOutputFileName, argv[2]);
		else
		{
			strcpy(szOutputFileName, szInputFileName);
			strcat(szOutputFileName, "_out.png");
		}
	}


	size_t nReqSize_in = NPngProc::readPngFile(szInputFileName, 0, 0, 0, 0);
	

	unsigned char* pInputBits = new unsigned char[nReqSize_in];
	if (!pInputBits)
	{
		printf("\nCan't allocate memory for image, required size is %u", nReqSize_in);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard_in(&pInputBits);


	unsigned char* pOutputBits1 = new unsigned char[nReqSize_in];
	if (!pOutputBits1)
	{
		printf("\nCan't allocate memory for image, required size is %u", nReqSize_in);
		return -1;
	}
	CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits1);


	size_t nWidth_in, nHeight_in, nWidth_mask, nHeight_mask;
	size_t nRetSize_in = NPngProc::readPngFileGray(szInputFileName, pInputBits, &nWidth_in, &nHeight_in);


	// Создаём маску
	std::string filenameStr = "circle.png";
	const char* filename = filenameStr.c_str();
	
	unsigned char* pMaskBits = Create_circle_mask(pInputBits, nWidth_in, nHeight_in, filename);;
	if (!pMaskBits)
	{
		printf("\nCan't allocate memory for mask, required size is %u", nWidth_in* nHeight_in * 4);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard_mask(&pMaskBits);
	size_t nRetSize_mask = NPngProc::readPngFileGray(filename, pMaskBits, &nWidth_mask, &nHeight_mask);

	// Применяем созданную маску к изображению
	GrayscaleCircle(pInputBits, pMaskBits, nWidth_in, nHeight_in, szOutputFileName);
	
	delete[] pInputBits;
	delete[] pOutputBits1;
	delete[] pMaskBits;
	return 0;
}

void GrayscaleCircle(unsigned char* pIn
			 , unsigned char* pMask
			 , size_t nWidth_in
			 , size_t nHeight_in
			 , const char* OutputFileName
)
{

	unsigned char* pOut = new unsigned char[nWidth_in * nHeight_in * 4]; // Добавить альфа-канал

	for (size_t y = 0; y < nHeight_in; ++y) {
		for (size_t x = 0; x < nWidth_in; ++x) {

			size_t pixelIndex = y * nWidth_in + x;

			pOut[(y * nWidth_in + x) * 4] = pIn[pixelIndex] * (pMask[pixelIndex] / 255); // Красный
			pOut[(y * nWidth_in + x) * 4 + 1] = pIn[pixelIndex] * (pMask[pixelIndex] / 255); // Зелёный
			pOut[(y * nWidth_in + x) * 4 + 2] = pIn[pixelIndex] * (pMask[pixelIndex] / 255); // Синий
			pOut[(y * nWidth_in + x) * 4 + 3] = 255 * (pMask[pixelIndex] / 255); // Альфа-канал

		}
	}

	NPngProc::WriteImageToFile(pOut, nWidth_in, nHeight_in, OutputFileName);
	return;
} 


unsigned char* Create_circle_mask(unsigned char* pIn
	, size_t nWidth
	, size_t nHeight
	, const char* OutputFileName
)
{
	unsigned char* pOut = new unsigned char[nWidth * nHeight * 4];

	double r = std::min(nWidth / 2, nHeight / 2);
	double x_ = (nWidth / 2);
	double y_ = (nHeight / 2);

	for (size_t y = 0; y < nHeight; ++y) {
		for (size_t x = 0; x < nWidth; ++x) {
			size_t pixelIndex = y * nWidth + x;
			double dx = x - x_;
			double dy = y - y_;
			double distance = dx*dx + dy*dy;
			if (distance <= r*r) {
				pOut[pixelIndex * 4] = 255;
				pOut[pixelIndex * 4 + 1] = 255;
				pOut[pixelIndex * 4 + 2] = 255;
				pOut[pixelIndex * 4 + 3] = 255;
			}
			else {
				pOut[pixelIndex * 4] = 0;
				pOut[pixelIndex * 4 + 1] = 0;
				pOut[pixelIndex * 4 + 2] = 0;
				pOut[pixelIndex * 4 + 3] = 255;
			}
		}
	}

	NPngProc::WriteImageToFile(pOut, nWidth, nHeight, OutputFileName);
	return pOut;
}
