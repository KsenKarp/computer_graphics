#include <opencv2/opencv.hpp> 
#include <string.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void FloydSteinberg(Mat& image, int n);


int main(int argc, char** argv)
{   
    unsigned int nBPP;
    nBPP = 8;
    Mat image = imread("cat_test.png", IMREAD_UNCHANGED);
    Mat image_g = imread("cat_test.png", IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cout << "Error while reading an image" << endl;
        cin.get(); 
        return -1;
    }

    if (image.channels() == 4) cvtColor(image, image, COLOR_RGBA2RGB);
    else if (image.channels() == 2 || image.channels() > 4)
    {
        cout << "Unsupported image channels number!" << endl;
        cout << image.channels() << endl;
        return -1;
    };



    int n = 1;
    FloydSteinberg(image, n);
    FloydSteinberg(image_g, n);

    waitKey(0);
    return 0;
}


void FloydSteinberg(Mat& image, int n)
{
    Size sz = image.size();
    int width = sz.width;
    int height = sz.height;

    int type = CV_8U;
    Mat out_img;


    if (image.channels() != 1) 
    { //color img
        out_img = Mat::zeros(image.size(), image.type());

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Vec3b pixel = image.at<Vec3b>(y, x);
                Vec3b quantized_pixel;
                for (int i = 0; i < 3; i++) {
                    quantized_pixel[i] = pixel[i] >> (8 - n) << (8 - n);
                }


                Vec3b error;
                for (int i = 0; i < 3; i++) {
                    error[i] = pixel[i] - quantized_pixel[i];
                }

                if (x < width - 1) {
                    Vec3b& right_pixel = image.at<Vec3b>(y, x + 1);
                    for (int i = 0; i < 3; i++) {
                        right_pixel[i] = min(255, max(0, right_pixel[i] + error[i] * 7 / 16));
                    }
                }
                if (y < height - 1) {
                    Vec3b& below_pixel = image.at<Vec3b>(y + 1, x);
                    for (int i = 0; i < 3; i++) {
                        below_pixel[i] = min(255, max(0, below_pixel[i] + error[i] * 5 / 16));
                    }
                    if (x < width - 1) {
                        Vec3b& below_right_pixel = image.at<Vec3b>(y + 1, x + 1);
                        for (int i = 0; i < 3; i++) {
                            below_right_pixel[i] = min(255,max(0, below_right_pixel[i] + error[i] * 1 / 16));
                        }
                    }
                    if (x > 0) {
                        Vec3b& below_left_pixel = image.at<Vec3b>(y + 1, x - 1);
                        for (int i = 0; i < 3; i++) {
                            below_left_pixel[i] = min(255, max(0, below_left_pixel[i] + error[i] * 3 / 16));
                        }
                    }
                }

                out_img.at<Vec3b>(y, x) = quantized_pixel;
            }
        }

        if (out_img.empty()) {
            cout << "Error while composing a new image" << endl;
            return;
        }

        imwrite("output_dithering_color.png", out_img);
        cout << "Output image saved to " << "\'output_dithering_color.png\'" << endl;

    }
    else 
    { //grayscale img
        out_img = Mat::zeros(sz, CV_8U);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {

                uchar pixel = image.at<uchar>(y, x);
                uchar quantized_pixel = pixel >> (8 - n) << (8 - n); //квантуем
                int error = pixel - quantized_pixel;

                if (x < width - 1) {
                    image.at<uchar>(y, x + 1) = min(255, max(0, image.at<uchar>(y, x + 1) + error * 7 / 16));
                }
                if (y < height - 1) {
                    image.at<uchar>(y + 1, x) = min(255, max(0, image.at<uchar>(y + 1, x) + error * 5 / 16));
                    if (x < image.cols - 1) {
                        image.at<uchar>(y + 1, x + 1) = min(255, max(0, image.at<uchar>(y + 1, x + 1) + error * 1 / 16));
                    }
                    if (x > 0) {
                        image.at<uchar>(y + 1, x - 1) = min(255, max(0, image.at<uchar>(y + 1, x - 1) + error * 3 / 16));
                    }
                }

                out_img.at<uchar>(y, x) = quantized_pixel;
            }
        }

        if (out_img.empty()) {
            cout << "Error while composing a new image" << endl;
            return;
        }

        imwrite("output_dithering_grayscale.png", out_img);
        cout << "Output image saved to " << "\'output_dithering_grayscale.png\'" << endl;

    }

    return;
}
