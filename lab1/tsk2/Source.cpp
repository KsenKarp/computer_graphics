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
uchar* pAlpha,
int nWidth1,
int nHeight1,
BlendMode mode
);


int main(int argc, char** argv)
{   
    unsigned int nBPP;
    nBPP = 8;
    Mat image1 = imread("cat_test_2.png", IMREAD_UNCHANGED);
    Mat image2 = imread("flag.png", IMREAD_UNCHANGED);
    Mat alpha = imread("grad1.png", IMREAD_UNCHANGED);

    if (image1.empty() || image2.empty() || alpha.empty())
    {
        cout << "Error while reading an image" << endl;
        cin.get(); 
        return -1;
    }

    Size sz1 = image1.size();
    Size sz2 = image2.size();
    Size sz_a = alpha.size();

    int width1 = sz1.width;
    int width2 = sz2.width;
    int width_a = sz_a.width;

    int height1 = sz1.height;
    int height2 = sz2.height;
    int height_a = sz_a.height;

    if (width1 != width2 || width1 != width_a || height1 != height2 || height1 != height_a) {
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


	Mat alpha_n;
	if (alpha.channels() == 3 || alpha.channels() == 1 || alpha.channels() == 4) {
		cvtColor(alpha, alpha_n, COLOR_BGR2GRAY);
	}
    else {
		cout << "Unsupported image channels number!" << endl;
		return -1;
	}

	int type = CV_8UC4;
	Mat output = Mat::zeros(height1, width1, type);
    BlendMode mode = Difference;
	Blending(output.data, image1_a.data, image2_a.data, alpha_n.data, width1, height1, mode);

    waitKey(0);
    return 0;
}


void Blending(uchar* pOut,
    uchar* pImage1,
    uchar* pImage2,
    uchar* pAlpha,
    int nWidth1,
    int nHeight1,
    BlendMode mode) 
{
    string mode_str;
    for (int y = 0; y < nHeight1; ++y) {
        for (int x = 0; x < nWidth1; ++x) {
            int pixelIndex_a = y * nWidth1 + x;
            int pixelIndex = y * nWidth1 * 4 + x * 4;

            float alpha_s = static_cast<float>(*(pAlpha + pixelIndex_a)) / 255.0;
            float alpha_b = 1.0 - alpha_s;

            uchar C_b_r = static_cast<int>(* (pImage1 + pixelIndex));
            uchar C_b_g = static_cast<int>(*(pImage1 + pixelIndex + 1));
            uchar C_b_b = static_cast<int>(*(pImage1 + pixelIndex + 2));

            uchar C_s_r = static_cast<int>(*(pImage2 + pixelIndex));
            uchar C_s_g = static_cast<int>(*(pImage2 + pixelIndex + 1));
            uchar C_s_b = static_cast<int>(*(pImage2 + pixelIndex + 2));

            uchar B_r, B_g, B_b;

            switch (mode) {
            case Normal:
                B_r = C_s_r;
                B_g = C_s_g;
                B_b = C_s_b;
                mode_str = (string)"normal";
                break;
            case Multiply:
                B_r = (uchar)(C_b_r * C_s_r / 255);
                B_g = (uchar)(C_b_g * C_s_g / 255);
                B_b = (uchar)(C_b_b * C_s_b / 255);
                mode_str = (string)"multiply";
                break;
            case Screen:
                B_r = (uchar)(255 * (1 - (1 - C_b_r / 255) * (1 - C_s_r / 255)));
                B_g = (uchar)(255 * (1 - (1 - C_b_g / 255) * (1 - C_s_g / 255)));
                B_b = (uchar)(255 * (1 - (1 - C_b_b / 255) * (1 - C_s_b / 255)));
                mode_str = (string)"screen";
                break;
            case Darken:
                B_r = (uchar)(std::min(C_b_r, C_s_r));
                B_g = (uchar)(std::min(C_b_g, C_s_g));
                B_b = (uchar)(std::min(C_b_b, C_s_b));
                mode_str = (string)"darken";
                break;
            case Lighten:
                B_r = (uchar)(std::max(C_b_r, C_s_r));
                B_g = (uchar)(std::max(C_b_g, C_s_g));
                B_b = (uchar)(std::max(C_b_b, C_s_b));
                mode_str = (string)"lighten";
                break;
            case Difference:
                B_r = (uchar)(std::abs(C_b_r - C_s_r));
                B_g = (uchar)(std::abs(C_b_g - C_s_g));
                B_b = (uchar)(std::abs(C_b_b - C_s_b));
                mode_str = (string)"difference";
                break;
            case ColorDodge:
                if (C_s_r < 255) {
                    B_r = (uchar)(std::min(255, 255 * C_b_r / (255 - C_s_r)));
                }
                else {
                    B_r = 255;
                }
                if (C_s_g < 255) {
                    B_g = (uchar)(std::min(255, 255 * C_b_g / (255 - C_s_g)));
                }
                else {
                    B_g = 255;
                }
                if (C_s_b < 255) {
                    B_b = (uchar)(std::min(255, 255 * C_b_b / (255 - C_s_b)));
                }
                else {
                    B_b = 255;
                }
                mode_str = (string)"colordodge";
                break;
            case ColorBurn:
                if (C_s_r > 0) {
                    B_r = (uchar)(255 - std::min(255, 255 * (255 - C_b_r) / C_s_r));
                }
                else {
                    B_r = 0;
                }
                if (C_s_g > 0) {
                    B_g = (uchar)(255 - std::min(255, 255 * (255 - C_b_g) / C_s_g));
                }
                else {
                    B_g = 0;
                }
                if (C_s_b > 0) {
                    B_b = (uchar)(255 - std::min(255, 255 * (255 - C_b_b) / C_s_b));
                }
                else {
                    B_b = 0;
                }
                mode_str = (string)"colorburn";
                break;
            case SoftLight:
                if (C_s_r < 128) {
                    B_r = (uchar)(C_b_r - (1 - 2 * C_s_r / 255) * C_b_r * (1 - C_b_r / 255));
                }
                else {
                    uchar D_r = (C_b_r <= 64) ? (uchar)((16 * C_b_r - 12) * C_b_r + 4)
                        * C_b_r : (uchar)(sqrt(C_b_r));
                    B_r = (uchar)(C_b_r + (2 * C_s_r / 255 - 1) * (D_r - C_b_r / 255) * 255);
                }
                if (C_s_g < 128) {
                    B_g = (uchar)(C_b_g - (1 - 2 * C_s_g / 255) * C_b_g * (1 - C_b_g / 255));
                }
                else {
                    uchar D_g = (C_b_g <= 64) ? (uchar)((16 * C_b_g - 12) * C_b_g + 4)
                        * C_b_g : (uchar)(sqrt(C_b_g));
                    B_g = (uchar)(C_b_g + (2 * C_s_g / 255 - 1) * (D_g - C_b_g / 255) * 255);
                }
                if (C_s_b < 128) {
                    B_b = (uchar)(C_b_b - (1 - 2 * C_s_b / 255) * C_b_b * (1 - C_b_b / 255));
                }
                else {
                    uchar D_b = (C_b_b <= 64) ? (uchar)((16 * C_b_b - 12) * C_b_b + 4)
                        * C_b_b : (uchar)(sqrt(C_b_b));
                    B_b = (uchar)(C_b_b + (2 * C_s_b / 255 - 1) * (D_b - C_b_b / 255) * 255);
                }
                mode_str = (string)"softlight";
                break;
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

            *(pOut + pixelIndex + 3) = 255;
        }
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