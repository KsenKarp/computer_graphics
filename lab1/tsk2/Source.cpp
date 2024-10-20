#include <opencv2/opencv.hpp> 
#include <string.h>
#include <iostream>

using namespace cv;
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


void Blending(uchar* pOut,
    uchar* pImage1,
    uchar* pImage2,
    uchar* pAlpha1,
    uchar* pAlpha2,
    int nWidth1,
    int nHeight1,
    BlendMode mode);


int main(int argc, char** argv)
{   
    unsigned int nBPP;
    nBPP = 8;
    Mat image1 = imread("cat_test_2.png", IMREAD_UNCHANGED);
    Mat image2 = imread("flag.png", IMREAD_UNCHANGED);
    Mat alpha1 = imread("grad.png", IMREAD_UNCHANGED);
    Mat alpha2 = imread("grad1.png", IMREAD_UNCHANGED);

    if (image1.empty() || image2.empty() || alpha1.empty() || alpha2.empty())
    {
        cout << "Error while reading an image" << endl;
        cin.get(); 
        return -1;
    }

    Size sz1 = image1.size();
    Size sz2 = image2.size();
    Size sz_a1 = alpha1.size();
    Size sz_a2 = alpha2.size();

    int width1 = sz1.width;
    int width2 = sz2.width;
    int width_a1 = sz_a1.width;
    int width_a2 = sz_a2.width;

    int height1 = sz1.height;
    int height2 = sz2.height;
    int height_a1 = sz_a1.height;
    int height_a2 = sz_a2.height;

    if (width1 != width2 || width1 != width_a1 || width1 != width_a2 || height1 != height2 || height1 != height_a1 || height1 != height_a2) {
        cout << "Images of different size" << endl;
        cin.get();
        return - 1;
    }

    Mat image1_a;
    if (image1.channels() == 1) {
        cvtColor(image1, image1_a, COLOR_GRAY2BGRA);
    }
    else if (image1.channels() == 3 || image1.channels() == 4) {
        cvtColor(image1, image1_a, COLOR_BGR2BGRA);
    }
    else {
        cout << "Unsupported image channels number!" << endl;
        return -1;
    };

    Mat image2_a;
    if (image2.channels() == 1) {
        cvtColor(image2, image2_a, COLOR_GRAY2BGRA);
    }
    else if (image2.channels() == 3 || image2.channels() == 4) {
        cvtColor(image2, image2_a, COLOR_BGR2BGRA);
    }
    else {
        cout << "Unsupported image channels number!" << endl;
        return -1;
    };


	Mat alpha1_n;
	if (alpha1.channels() == 3 || alpha1.channels() == 1 || alpha1.channels() == 4) {
		cvtColor(alpha1, alpha1_n, COLOR_BGR2GRAY);
	}
    else {
		cout << "Unsupported image channels number!" << endl;
		return -1;
	}

    Mat alpha2_n;
    if (alpha2.channels() == 3 || alpha2.channels() == 1 || alpha2.channels() == 4) {
        cvtColor(alpha2, alpha2_n, COLOR_BGR2GRAY);
    }
    else {
        cout << "Unsupported image channels number!" << endl;
        return -1;
    }

	int type = CV_8UC4;
	Mat output = Mat::zeros(height1, width1, type);
    BlendMode mode = ColorDodge;
	Blending(output.data, image1_a.data, image2_a.data, alpha1_n.data, alpha2_n.data, width1, height1, mode);

    waitKey(0);
    return 0;
}


void Blending(uchar* pOut,
    uchar* pImage1,
    uchar* pImage2,
    uchar* pAlpha1,
    uchar* pAlpha2,
    int nWidth1,
    int nHeight1,
    BlendMode mode) 
{
    string mode_str;
    switch (mode) {
    case Normal:
        mode_str = (string)"normal";
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;
                B_r = C_s_r;
                B_g = C_s_g;
                B_b = C_s_b;

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }

        break;
    case Multiply:
        mode_str = (string)"multiply";
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                B_r = (double)(C_b_r * C_s_r / 255.0);
                B_g = (double)(C_b_g * C_s_g / 255.0);
                B_b = (double)(C_b_b * C_s_b / 255.0);

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        break;
    case Screen:
        mode_str = (string)"screen";
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                B_r = (double)(255.0 * (1 - (1 - C_b_r / 255.0) * (1 - C_s_r / 255.0)));
                B_g = (double)(255.0 * (1 - (1 - C_b_g / 255.0) * (1 - C_s_g / 255.0)));
                B_b = (double)(255.0 * (1 - (1 - C_b_b / 255.0) * (1 - C_s_b / 255.0)));

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        break;
    case Darken:
        mode_str = (string)"darken";
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                B_r = (double)(std::min(C_b_r, C_s_r));
                B_g = (double)(std::min(C_b_g, C_s_g));
                B_b = (double)(std::min(C_b_b, C_s_b));

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        break;
    case Lighten:
        mode_str = (string)"lighten";
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                B_r = (double)(std::max(C_b_r, C_s_r));
                B_g = (double)(std::max(C_b_g, C_s_g));
                B_b = (double)(std::max(C_b_b, C_s_b));

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        break;
    case Difference:
        mode_str = (string)"difference";
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                B_r = (double)(std::abs(C_b_r - C_s_r));
                B_g = (double)(std::abs(C_b_g - C_s_g));
                B_b = (double)(std::abs(C_b_b - C_s_b));

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;

            }
        }
        break;
    case ColorDodge:
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                if (C_s_r < 255) {
                    B_r = (double)(std::min(255.0, 255.0 * C_b_r / (255.0 - C_s_r)));
                }
                else {
                    B_r = 255.0;
                }
                if (C_s_g < 255) {
                    B_g = (double)(std::min(255.0, 255.0 * C_b_g / (255.0 - C_s_g)));
                }
                else {
                    B_g = 255.0;
                }
                if (C_s_b < 255) {
                    B_b = (double)(std::min(255.0, 255.0 * C_b_b / (255.0 - C_s_b)));
                }
                else {
                    B_b = 255.0;
                }

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        mode_str = (string)"colordodge";
        break;
    case ColorBurn:
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                if (C_s_r > 0) {
                    B_r = (double)(255.0 - std::min(255.0, 255.0 * (255.0 - C_b_r) / C_s_r));
                }
                else {
                    B_r = 0.0;
                }
                if (C_s_g > 0) {
                    B_g = (double)(255.0 - std::min(255.0, 255.0 * (255.0 - C_b_g) / C_s_g));
                }
                else {
                    B_g = 0.0;
                }
                if (C_s_b > 0) {
                    B_b = (double)(255.0 - std::min(255.0, 255.0 * (255.0 - C_b_b) / C_s_b));
                }
                else {
                    B_b = 0.0;
                }

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        mode_str = (string)"colorburn";
        break;
    case SoftLight:
        for (int y = 0; y < nHeight1; ++y) {
            for (int x = 0; x < nWidth1; ++x) {
                int pixelIndex_a = y * nWidth1 + x;
                int pixelIndex = y * nWidth1 * 4 + x * 4;

                float alpha_s = static_cast<float>(*(pAlpha1 + pixelIndex_a)) / 255.0;
                float alpha_b = 1.0 - static_cast<float>(*(pAlpha2 + pixelIndex_a)) / 255.0;

                double C_b_r = static_cast<double>(*(pImage1 + pixelIndex));
                double C_b_g = static_cast<double>(*(pImage1 + pixelIndex + 1));
                double C_b_b = static_cast<double>(*(pImage1 + pixelIndex + 2));

                double C_s_r = static_cast<double>(*(pImage2 + pixelIndex));
                double C_s_g = static_cast<double>(*(pImage2 + pixelIndex + 1));
                double C_s_b = static_cast<double>(*(pImage2 + pixelIndex + 2));

                double B_r, B_g, B_b;


                if (C_s_r < 128) {
                    B_r = (double)(C_b_r - (1 - 2 * C_s_r / 255.0) * C_b_r * (1 - C_b_r / 255.0));
                }
                else {
                    double D_r = (C_b_r <= 64) ? (double)((16.0 * C_b_r - 12) * C_b_r + 4)
                        * C_b_r : (double)(sqrt(C_b_r));
                    B_r = (double)(C_b_r + (2 * C_s_r / 255.0 - 1) * (D_r - C_b_r / 255.0) * 255.0);
                }
                if (C_s_g < 128) {
                    B_g = (double)(C_b_g - (1 - 2 * C_s_g / 255.0) * C_b_g * (1 - C_b_g / 255.0));
                }
                else {
                    double D_g = (C_b_g <= 64) ? (double)((16.0 * C_b_g - 12) * C_b_g + 4)
                        * C_b_g : (double)(sqrt(C_b_g));
                    B_g = (double)(C_b_g + (2 * C_s_g / 255.0 - 1) * (D_g - C_b_g / 255.0) * 255.0);
                }
                if (C_s_b < 128) {
                    B_b = (double)(C_b_b - (1 - 2 * C_s_b / 255.0) * C_b_b * (1 - C_b_b / 255.0));
                }
                else {
                    double D_b = (C_b_b <= 64) ? (double)((16.0 * C_b_b - 12) * C_b_b + 4)
                        * C_b_b : (uchar)(sqrt(C_b_b));
                    B_b = (double)(C_b_b + (2 * C_s_b / 255.0 - 1) * (D_b - C_b_b / 255.0) * 255.0);
                }

                *(pOut + pixelIndex) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_r +
                    (1 - alpha_b) * (alpha_s)*C_s_r +
                    (alpha_s) * (alpha_b)*B_r);
                if (*(pOut + pixelIndex) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 1) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_g +
                    (1 - alpha_b) * (alpha_s)*C_s_g +
                    (alpha_s) * (alpha_b)*B_g);
                if (*(pOut + pixelIndex + 1) > 255) *(pOut + pixelIndex) = 255;

                *(pOut + pixelIndex + 2) = (uchar)((1 - alpha_s) * (alpha_b)*C_b_b +
                    (1 - alpha_b) * (alpha_s)*C_s_b +
                    (alpha_s) * (alpha_b)*B_b);
                if (*(pOut + pixelIndex + 2) > 255) *(pOut + pixelIndex) = 255;
                if (*(pOut + pixelIndex + 2) < 0) *(pOut + pixelIndex) = 0;

                *(pOut + pixelIndex + 3) = 255;
            }
        }
        mode_str = (string)"softlight";
        break;
    }


    Mat outputImage(nHeight1, nWidth1, CV_8UC4, pOut);

    if (outputImage.empty()) {
        cout << "Error while composing a new image" << endl;
        return;
    }

    string path = "output_" + mode_str + ".png";
    imwrite(path, outputImage);
    cout << "Output image saved to " << "\'output.png\'" << endl;
    return;
}
