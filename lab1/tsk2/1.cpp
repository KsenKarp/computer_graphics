// pngtest.cpp : Defines the entry point for the console application.
//


#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

enum BlendMode {
	Normal,
	Multiply,
	Screen,
	Darken,
	Lighten,
	Difference,
	ColorDodge,
	ColorBurn,
	SoftLight
};

void Blending(unsigned char* pIn1
	, unsigned char* pIn2
	, unsigned char* pIn3
	, unsigned char* pOut
	, size_t nWidth
	, size_t nHeight
	, const char* OutputFileName
	, BlendMode mode
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
	char	szInputFileName1[256];
	char	szInputFileName2[256];
	char	szInputFileName3[256];
	char    szOutputFileName[256];
	if (argc < 4)
		printf("\nformat: pngtest <input_file1> <input_file2> <input_file3> [<output_file>]");
	else
	{
		strcpy(szInputFileName1, argv[1]);
		strcpy(szInputFileName2, argv[2]);
		strcpy(szInputFileName3, argv[3]);
		if (argc > 4)
			strcpy(szOutputFileName, argv[4]);
		else
		{
			strcpy(szOutputFileName, szInputFileName1);
			strcat(szOutputFileName, szInputFileName2);
			strcat(szOutputFileName, "_out.png");
		}
	}


	size_t nReqSize_in1 = NPngProc::readPngFile(szInputFileName1, 0, 0, 0, 0);
	size_t nReqSize_in2 = NPngProc::readPngFile(szInputFileName2, 0, 0, 0, 0);
	size_t nReqSize_in3 = NPngProc::readPngFile(szInputFileName3, 0, 0, 0, 0);
	if (nReqSize_in1 == NPngProc::PNG_ERROR || nReqSize_in2 == NPngProc::PNG_ERROR || nReqSize_in3 == NPngProc::PNG_ERROR)
	{
		printf("\nError occurred while png file was read");
		return -1;
	}

	unsigned char* pInputBits1 = new unsigned char[nReqSize_in1];
	if (!pInputBits1)
	{
		printf("\nCan't allocate memory for image 1, required size is %u", nReqSize_in1);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard_in1(&pInputBits1);

	unsigned char* pInputBits2 = new unsigned char[nReqSize_in2];
	if (!pInputBits2)
	{
		printf("\nCan't allocate memory for image 2, required size is %u", nReqSize_in2);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard_in2(&pInputBits2);

	unsigned char* pInputBits3 = new unsigned char[nReqSize_in3];
	if (!pInputBits3)
	{
		printf("\nCan't allocate memory for image 3, required size is %u", nReqSize_in3);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard_in3(&pInputBits3);


	size_t nWidth_in1, nHeight_in1, nWidth_in2, nHeight_in2, nWidth_in3, nHeight_in3;
	size_t nRetSize_in1 = NPngProc::readPngFileGray(szInputFileName1, pInputBits1, &nWidth_in1, &nHeight_in1);
	size_t nRetSize_in2 = NPngProc::readPngFileGray(szInputFileName2, pInputBits2, &nWidth_in2, &nHeight_in2);
	size_t nRetSize_in3 = NPngProc::readPngFileGray(szInputFileName3, pInputBits3, &nWidth_in3, &nHeight_in3);

	unsigned char* pOutputBits1 = new unsigned char[nWidth_in1 * nWidth_in2 * 4];
	if (!pOutputBits1)
	{
		printf("\nCan't allocate memory for output image, required size is %u", nWidth_in1* nWidth_in2 * 4);
		return -1;
	}
	CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits1);


	//Проверим, что все изображения одного размера:
	if (nWidth_in1 != nWidth_in2 || nWidth_in1 != nWidth_in3 || nHeight_in1 != nHeight_in2 || nHeight_in1 != nHeight_in3) 
	{
		printf("\nImages are not of the same size");
		return -1;
	}

	BlendMode mode = Screen;
	Blending(pInputBits1, pInputBits2, pInputBits3, pOutputBits1, nWidth_in1, nHeight_in1, szOutputFileName, mode);
	
	delete[] pInputBits1;
	delete[] pInputBits2;
	delete[] pInputBits3;
	delete[] pOutputBits1;

	return 0;
}



// Смешиваются первое(нижнее) и второе(верхнее) изображение через третье в качестве альфа-канала
/*
void Blending(unsigned char* pIn1
	, unsigned char* pIn2
	, unsigned char* pIn3
	, unsigned char* pOut
	, size_t nWidth
	, size_t nHeight
	, const char* OutputFileName
	, BlendMode mode
)
{
	for (size_t y = 0; y < nHeight; ++y) {
		for (size_t x = 0; x < nWidth; ++x) {
			size_t pixelIndex = y * nWidth + x;

			double alpha_s = pIn3[pixelIndex * 4 + 3] / 255.0;
			double alpha_b = (1 - alpha_s);

			double C_b_r = pIn1[pixelIndex * 4] / 255.0;
			double C_b_g = pIn1[pixelIndex * 4 + 1] / 255.0;
			double C_b_b = pIn1[pixelIndex * 4 + 2] / 255.0;

			double C_s_r = pIn2[pixelIndex * 4] / 255.0;
			double C_s_g = pIn2[pixelIndex * 4 + 1] / 255.0;
			double C_s_b = pIn2[pixelIndex * 4 + 2] / 255.0;

			double B_r, B_g, B_b;

			switch (mode) {
			case Normal:
				B_r = C_s_r;
				B_g = C_s_g;
				B_b = C_s_b;
				break;
			case Multiply:
				B_r = (C_b_r * C_s_r);
				B_g = (C_b_g * C_s_g);
				B_b = (C_b_b * C_s_b);
				break;
			case Screen:
				B_r = (1 - (1 - C_b_r) * (1 - C_s_r));
				B_g = (1 - (1 - C_b_g) * (1 - C_s_g));
				B_b = (1 - (1 - C_b_b) * (1 - C_s_b));
				break;
			case Darken:
				B_r = (std::min(C_b_r, C_s_r));
				B_g = (std::min(C_b_g, C_s_g));
				B_b = (std::min(C_b_b, C_s_b));
				break;
			case Lighten:
				B_r = (std::max(C_b_r, C_s_r));
				B_g = (std::max(C_b_g, C_s_g));
				B_b = (std::max(C_b_b, C_s_b));
				break;
			case Difference:
				B_r = (std::abs(C_b_r - C_s_r));
				B_g = (std::abs(C_b_g - C_s_g));
				B_b = (std::abs(C_b_b - C_s_b));
				break;
			case ColorDodge:
				if (C_s_r < 1) {
					B_r = (std::min(1.0, C_b_r / (1 - C_s_r)));
				}
				else {
					B_r = 1.0;
				}
				if (C_s_g < 1) {
					B_g = (unsigned char)(std::min(1.0, C_b_g / (1 - C_s_g)));
				}
				else {
					B_g = 1.0;
				}
				if (C_s_b < 1) {
					B_b = (unsigned char)(std::min(1.0, C_b_b / (1 - C_s_b)));
				}
				else {
					B_b = 1.0;
				}
				break;
			case ColorBurn:
				if (C_s_r > 0) {
					B_r = (unsigned char)(1 - std::min(1.0, (1 - C_b_r) / C_s_r));
				}
				else {
					B_r = 0;
				}
				if (C_s_g > 0) {
					B_g = (unsigned char)(1 - std::min(1.0, (1 - C_b_g) / C_s_g));
				}
				else {
					B_g = 0;
				}
				if (C_s_b > 0) {
					B_b = (unsigned char)(1 - std::min(1.0, (1 - C_b_b) / C_s_b));
				}
				else {
					B_b = 0;
				}
				break;
			case SoftLight:
				if (C_s_r < 0.5) {
					B_r = (unsigned char)(C_b_r - (1 - 2 * C_s_r) * C_b_r * (1 - C_b_r));
				}
				else {
					unsigned char D_r = (C_b_r <= 0.25) ? (unsigned char)((16 * C_b_r - 12) * C_b_r + 4) * C_b_r : (unsigned char)(sqrt(C_b_r));
					B_r = (unsigned char)(C_b_r + (2 * C_s_r - 1) * (D_r - C_b_r));
				}
				if (C_s_g < 0.5) {
					B_g = (unsigned char)(C_b_g - (1 - 2 * C_s_g) * C_b_g * (1 - C_b_g));
				}
				else {
					unsigned char D_g = (C_b_g <= 0.25) ? (unsigned char)((16 * C_b_g - 12) * C_b_g + 4) * C_b_g : (unsigned char)(sqrt(C_b_g));
					B_g = (unsigned char)(C_b_g + (2 * C_s_g - 1) * (D_g - C_b_g));
				}
				if (C_s_b < 0.5) {
					B_b = (unsigned char)(C_b_b - (1 - 2 * C_s_b) * C_b_b * (1 - C_b_b));
				}
				else {
					unsigned char D_b = (C_b_b <= 0.25) ? (unsigned char)((16 * C_b_b - 12) * C_b_b + 4) * C_b_b : (unsigned char)(sqrt(C_b_b));
					B_b = (unsigned char)(C_b_b + (2 * C_s_b - 1) * (D_b - C_b_b));
				}
				break;
			}

			pOut[pixelIndex * 4] = (unsigned char)(
				((1 - alpha_s) * (alpha_b)*C_b_r
					+ (1 - alpha_b) * (alpha_s)*C_s_r
					+ (alpha_s) * (alpha_b)*B_r) * 255
				);
			pOut[pixelIndex * 4 + 1] = (unsigned char)(
				((1 - alpha_s) * (alpha_b)*C_b_g
					+ (1 - alpha_b) * (alpha_s)*C_s_g
					+ (alpha_s) * (alpha_b)*B_g) * 255
				);
			pOut[pixelIndex * 4 + 2] = (unsigned char)(
				((1 - alpha_s) * (alpha_b)*C_b_b
					+ (1 - alpha_b) * (alpha_s)*C_s_b
					+ (alpha_s) * (alpha_b)*B_b) * 255
				);
			pOut[pixelIndex * 4 + 3] = (unsigned char)(alpha_s * 255);
		}
	}
	// save it to a file
	NPngProc::WriteImageToFile(pOut, nWidth, nHeight, OutputFileName);

	return;
}
*/


void Blending(
	unsigned char* pImage1,
	unsigned char* pImage2,
	unsigned char* pAlpha,
	unsigned char* pOut,
	size_t nWidth1,
	size_t nHeight1,
	const char* OutputFileName,
	BlendMode mode)
{

	// iterate over each pixel of the original image
	for (size_t y = 0; y < nHeight1; ++y) {
		for (size_t x = 0; x < nWidth1; ++x) {
			size_t pixelIndex = y * nWidth1 + x;

			unsigned char alpha_s = pAlpha[pixelIndex] / 255;
			unsigned char alpha_b = 1 - alpha_s;

			unsigned char C_b = pImage1[pixelIndex];
			unsigned char C_s = pImage2[pixelIndex];
			unsigned char B;

			switch (mode) {
			case Normal:
				B = C_s;
				break;
			case Multiply:
				B = (unsigned char)(C_b * C_s / 255);
				break;
			case Screen:
				B = (unsigned char)(255 * (1 - (1 - C_b / 255) * (1 - C_s / 255)));
				break;
			case Darken:
				B = (unsigned char)(std::min(C_b, C_s));
				break;
			case Lighten:
				B = (unsigned char)(std::max(C_b, C_s));
				break;
			case Difference:
				B = (unsigned char)(std::abs(C_b - C_s));
				break;
			case ColorDodge:
				if (C_s < 255) {
					B = (unsigned char)(std::min(255, 255 * C_b / (255 - C_s)));
				}
				else {
					B = 255;
				}
				break;
			case ColorBurn:
				if (C_s > 0) {
					B = (unsigned char)(255 - std::min(255, 255 * (255 - C_b) / C_s));
				}
				else {
					B = 0;
				}
				break;
			case SoftLight:
				if (C_s < 128) {
					B = (unsigned char)(C_b - (1 - 2 * C_s / 255) * C_b * (1 - C_b / 255));
				}
				else {
					unsigned char D = (C_b <= 64) ? (unsigned char)((16 * C_b - 12) * C_b + 4) * C_b : (unsigned char)(sqrt(C_b));
					B = (unsigned char)(C_b + (2 * C_s / 255 - 1) * (D - C_b / 255) * 255);
				}
				break;
			}

			pOut[pixelIndex * 4] = (unsigned char)((1 - alpha_s) * (alpha_b)*C_b +
				(1 - alpha_b) * (alpha_s)*C_s +
				(alpha_s) * (alpha_b)*B);

			pOut[pixelIndex * 4 + 1] = (unsigned char)((1 - alpha_s) * (alpha_b)*C_b +
				(1 - alpha_b) * (alpha_s)*C_s +
				(alpha_s) * (alpha_b)*B);

			pOut[pixelIndex * 4 + 2] = (unsigned char)((1 - alpha_s) * (alpha_b)*C_b +
				(1 - alpha_b) * (alpha_s)*C_s +
				(alpha_s) * (alpha_b)*B);

			pOut[pixelIndex * 4 + 3] = (unsigned char)((1 - alpha_s) * (alpha_b)*C_b +
				(1 - alpha_b) * (alpha_s)*C_s +
				(alpha_s) * (alpha_b)*B);
		}
	}
	// save it to a file
	NPngProc::WriteImageToFile(pOut, nWidth1, nHeight1, OutputFileName);

	return;
}
