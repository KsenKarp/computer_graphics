#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

//-----------------------------------------------------------------------------------------------------------------------------------//
//                                                              VAR10                                                                //
//-----------------------------------------------------------------------------------------------------------------------------------//


void set_pixel(Mat& img, int x, int y, int color) {
    int r = color & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = (color >> 16) & 0xFF;
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
        img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}
void draw_line(Mat& img, int x1, int y1, int x2, int y2, int color);
double distanсe_(Point p1, Point p2);


//task1
void draw_polygon(Mat& img, vector<Point> points, int color) {
    for (int i = 0; i < points.size(); i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];
        draw_line(img, p1.x, p1.y, p2.x, p2.y, color);
    }
}

bool is_point_on_interval(Point p, Point a, Point b) {
    return (min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)) &&
        (min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y)) &&
        ((b.x - a.x) * (p.y - a.y) == (b.y - a.y) * (p.x - a.x));
}

double get_angle(Point p1, Point p2, Point p3);
bool is_convex(vector<Point> points);
bool intersect(Point p1, Point p2, Point p3, Point p4);
bool is_simple(vector<Point> points);
bool is_clockwise(vector<Point>& vertex);
void reverse_polygon(vector<Point>& polygon);
Point get_intersection(Point a, Point b, Point c, Point d);

vector <Point> weiler_atherton_modification(vector<Point> vertexS, vector<Point> vertexC);


//task2
float max_dist(Point P0, Point P1, Point P2, Point P3) {
    float d1 = distanсe_(P0 - 2 * P1 + P2, Point(0, 0));
    float d2 = distanсe_(P1 - 2 * P2 + P3, Point(0, 0));
    return max(d1, d2);
}
Point bezier_points(Point P0, Point P1, Point P2, Point P3, double t);
void bezier_curve(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3);
bool check_smoothness(Point& P2, Point& P3, Point& Q1);
void composite_bezier_curve(Mat& img, vector<Point>& controlPoints);


//task3
void RGBtoHSL(float r, float g, float b, float& h, float& s, float& l);



int main(int argc, char** argv)
{
    //task1:
    /*
    vector<Point> vertexS_first = { Point(50, 300), Point(50, 100), Point(400, 100), Point(400, 300) };
    vector<Point> vertexC_first = { Point(200, 113), Point(300, 113), Point(420, 389), Point(256, 499), Point(150, 350) };

    vector <Point> result_first = weiler_atherton_modification(vertexS_first, vertexC_first);
    Mat img_task1_1(512, 512, CV_8UC3, Scalar(255, 255, 255));
    int color_main = 255;
    int color_basic = 0;
    draw_polygon(img_task1_1, vertexS_first, color_basic);
    draw_polygon(img_task1_1, vertexC_first, color_basic);
    draw_polygon(img_task1_1, result_first, color_main);
    imwrite("task1_1.png", img_task1_1);

    vector<Point> vertexS_second = { Point(70, 240), Point(350, 300), Point(100, 320) };
    vector<Point> vertexC_second = { Point(200, 113), Point(300, 113), Point(420, 389), Point(256, 499), Point(150, 350) };

    vector <Point> result_second = weiler_atherton_modification(vertexS_second, vertexC_second);
    Mat img_task1_2(512, 512, CV_8UC3, Scalar(255, 255, 255));
    draw_polygon(img_task1_2, vertexS_second, color_basic);
    draw_polygon(img_task1_2, vertexC_second, color_basic);
    draw_polygon(img_task1_2, result_second, color_main);
    imwrite("task1_2.png", img_task1_2);

    vector<Point> vertexS_third = { Point(100, 100), Point(100, 200), Point(200, 200), Point(200, 100) };
    vector<Point> vertexC_third = { Point(50, 50), Point(50, 250), Point(250, 250), Point(250, 50) };

    vector <Point> result_third = weiler_atherton_modification(vertexS_third, vertexC_third);
    Mat img_task1_3(512, 512, CV_8UC3, Scalar(255, 255, 255));
    draw_polygon(img_task1_3, vertexS_third, color_basic);
    draw_polygon(img_task1_3, vertexC_third, color_basic);
    draw_polygon(img_task1_3, result_third, color_main);
    imwrite("task1_3.png", img_task1_3);
    //Output: Polygons have no points of intersection.

    vector<Point> vertexS_fourth = { Point(100, 100), Point(200, 100), Point(100, 200), Point(200, 200) };
    vector<Point> vertexC_fourth = { Point(50, 50), Point(50, 250), Point(250, 250), Point(250, 50) };

    vector <Point> result_fourth = weiler_atherton_modification(vertexS_fourth, vertexC_fourth);
    Mat img_task1_4(512, 512, CV_8UC3, Scalar(255, 255, 255));
    draw_polygon(img_task1_4, vertexS_fourth, color_basic);
    draw_polygon(img_task1_4, vertexC_fourth, color_basic);
    draw_polygon(img_task1_4, result_fourth, color_main);
    imwrite("task1_4.png", img_task1_4);
    //Output: The algorithm would not work because the rectangle is not convex.
    */


    //task2:
    /*
    vector<Point> controlPoints = {
    Point(50, 200),
    Point(100, 100),
    Point(200, 100),
    Point(250, 200),

    Point(300, 300),
    Point(400, 300),
    Point(450, 200)
    };
    Mat img(500, 500, CV_8UC3, Scalar(255, 255, 255));
    composite_bezier_curve(img, controlPoints);
    imwrite("Composite_Bezier_curve.png", img);

    vector<Point> circlePoints = {
        Point(250, 100),
        Point(332.84, 100),
        Point(400, 167.16),
        Point(400, 250),

        Point(400, 332.84),
        Point(332.84, 400),
        Point(250, 400),

        Point(167.16, 400),
        Point(100, 332.84),
        Point(100, 250),

        Point(100, 167.16),
        Point(167.16, 100),
        Point(250, 100)
    };
    Mat img1(500, 500, CV_8UC3, Scalar(255, 255, 255));
    composite_bezier_curve(img1, circlePoints);
    imwrite("bezier_circle.png", img1);
    */


    //task3:
    Mat image = imread("cat_test.png");

    if (image.type() != CV_8UC3) {
        cout << "Image is not 24 bpp (CV_8UC3)!" << endl;
        return -1;
    }
    
    int hBins = 360;
    int sBins = 256;
    int lBins = 256;
    float hRange[] = { 0, 360 };
    float sRange[] = { 0, 1 };
    float lRange[] = { 0, 1 };

    const float* hHistRange = { hRange };
    const float* sHistRange = { sRange };
    const float* lHistRange = { lRange };

    Mat hHist, sHist, lHist;
    hHist = Mat::zeros(1, hBins, CV_32F);
    sHist = Mat::zeros(1, sBins, CV_32F);
    lHist = Mat::zeros(1, lBins, CV_32F);

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);

            float h, s, l;
            RGBtoHSL(pixel[2], pixel[1], pixel[0], h, s, l);

            int hBinIndex = (int)h % hBins;
            int sBinIndex = (int)(s * (sBins - 1));
            int lBinIndex = (int)(l * (lBins - 1));

            hHist.at<float>(0, hBinIndex)++;
            sHist.at<float>(0, sBinIndex)++;
            lHist.at<float>(0, lBinIndex)++;
        }
    }
    normalize(hHist, hHist, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(sHist, sHist, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(lHist, lHist, 0, 1, NORM_MINMAX, -1, Mat());

    int histWidth = 512;
    int histHeight = 512;
    int hBinWidth = cvRound((double)histWidth / hBins);
    int sBinWidth = cvRound((double)histWidth / sBins);
    int lBinWidth = cvRound((double)histWidth / lBins);

    Mat hHistImage(histHeight, histWidth, CV_8UC3, Scalar(255, 255, 255));
    Mat sHistImage(histHeight, histWidth, CV_8UC3, Scalar(255, 255, 255));
    Mat lHistImage(histHeight, histWidth, CV_8UC3, Scalar(255, 255, 255));

    for (int i = 0; i < hBins; i++) {
        int barHeight = cvRound(hHist.at<float>(0, i) * histHeight);
        rectangle(hHistImage, Point(hBinWidth * i, histHeight),
            Point(hBinWidth * (i + 1), histHeight - barHeight),
            Scalar(255, 0, 0), FILLED);
    }

    for (int i = 0; i < sBins; i++) {
        int barHeight = cvRound(sHist.at<float>(0, i) * histHeight);
        rectangle(sHistImage, Point(sBinWidth * i, histHeight),
            Point(sBinWidth * (i + 1), histHeight - barHeight),
            Scalar(0, 255, 0), FILLED);
    }

    for (int i = 0; i < lBins; i++) {
        int barHeight = cvRound(lHist.at<float>(0, i) * histHeight);
        rectangle(lHistImage, Point(lBinWidth * i, histHeight),
            Point(lBinWidth * (i + 1), histHeight - barHeight),
            Scalar(0, 0, 255), FILLED);
    }

    imshow("Гистограмма H", hHistImage);
    imshow("Гистограмма S", sHistImage);
    imshow("Гистограмма L", lHistImage);

    imwrite("H_hist.png", hHistImage);
    imwrite("S_hist.png", sHistImage);
    imwrite("L_hist.png", lHistImage);

    waitKey(0);

    return 0;
}


void draw_line(Mat& img, int x1, int y1, int x2, int y2, int color) {
    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int error;

    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    if (ix == -1) dx = -dx; //distance is greater than zero

    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    if (iy == -1) dy = -dy;

    if (dx >= dy) {
        error = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            set_pixel(img, x, y, color);
            if (error >= (iy >= 0 ? 0 : 1)) {
                y += iy;
                error -= 2 * dx;
            }
            x += ix;
            error += dy * 2;
        }
    }
    else {
        error = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            set_pixel(img, x, y, color);
            if (error >= (ix >= 0 ? 0 : 1)) {
                x += ix;
                error -= 2 * dy;
            }
            y += iy;
            error += dx * 2;
        }
    }
}

bool intersect(Point p1, Point p2, Point p3, Point p4) {
    double x1 = p1.x;
    double y1 = p1.y;
    double x2 = p2.x;
    double y2 = p2.y;
    double x3 = p3.x;
    double y3 = p3.y;
    double x4 = p4.x;
    double y4 = p4.y;
    double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denominator == 0) {
        return false;
    }
    else {
        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
        double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;
        return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
    }
}

bool is_simple(vector<Point> points) {
    int n = points.size();
    if (n == 3) {
        return true;
    }
    else if (n < 3) {
        cout << "invalid points vector" << endl;
        return false;
    }

    for (int i = 0; i < n - 3; i++) {
        if (intersect(points[i], points[i + 1],
            points[i + 2], points[i + 3])) {
            return false;
        }
    }
    return true;
}

double get_angle(Point p1, Point p2, Point p3) {
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dx2 = p3.x - p2.x;
    double dy2 = p3.y - p2.y;
    double angle = atan2(dy2, dx2) - atan2(dy1, dx1);

    if (angle > M_PI) angle -= 2 * M_PI;
    if (angle < -M_PI) angle += 2 * M_PI;

    return angle * 180 / M_PI;
}

bool is_convex(vector<Point> points) {
    int n = points.size();
    if ((n < 3) || (!is_simple(points))) {
        return false;
    }

    bool isConvex = true;
    double prev_angle = 0;

    for (int i = 0; i < n; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];
        Point p3 = points[(i + 2) % n];
        double angle = get_angle(p1, p2, p3);

        if (i > 0) {
            if ((angle > 0) != (prev_angle > 0)) {
                isConvex = false;
                break;
            }
        }
        prev_angle = angle;
    }

    return isConvex;
}

bool is_clockwise(vector<Point>& vertex) {
    double areaAll = 0;
    for (int i = 0; i < vertex.size(); i++)
    {
        int j = (i + 1) % vertex.size();
        areaAll += (vertex[j].x - vertex[i].x) * (vertex[j].y + vertex[i].y);
    }
    return (areaAll > 0) ? true : (areaAll < 0) ? false : true;
}

void reverse_polygon(vector<Point>& polygon) {
    reverse(polygon.begin(), polygon.end());
}

Point get_intersection(Point a, Point b, Point c, Point d) {
    double a1 = b.y - a.y;
    double b1 = a.x - b.x;
    double c1 = a1 * a.x + b1 * a.y;

    double a2 = d.y - c.y;
    double b2 = c.x - d.x;
    double c2 = a2 * c.x + b2 * c.y;

    double det = a1 * b2 - a2 * b1;
    Point p(static_cast<int>((c1 * b2 - c2 * b1) / det), static_cast<int>((a1 * c2 - a2 * c1) / det));
    return p;
}

double distanсe_(Point p1, Point p2)
{
    return  (pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

int get_previous_index_circular(int idx, int n) {
    if (idx == 0) {
        return (n - 1);
    }
    return (idx - 1);
}

vector<Point> slice(vector<Point>& v, int m, int n)
{
    vector<Point> vec(n - m + 1);
    copy(v.begin() + m, v.begin() + n + 1, vec.begin());
    return vec;
}

vector <Point> weiler_atherton_modification(vector<Point> vertexS, vector<Point> vertexC)
{
    vector <Point> result;
    if (is_convex(vertexS) == false || is_convex(vertexC) == false)
    {
        cout << "The algorithm would not work because the rectangle is not convex" << endl;
        return result;
    }
    if (is_clockwise(vertexS) == false)
    {
        reverse_polygon(vertexS);
    }
    if (is_clockwise(vertexC) == false)
    {
        reverse_polygon(vertexC);
    }
    vector<int> positionS;
    vector<int> positionC;
    vector<Point> intersect_point;
    int count = 0;
    for (int i = 0; i < vertexS.size(); i++)
    {
        for (int j = 0; j < vertexC.size(); j++)
        {
            if (intersect(vertexS[i % vertexS.size()], vertexS[(i + 1) % vertexS.size()], vertexC[j % vertexC.size()], vertexC[(j + 1) % vertexC.size()]))
            {
                positionS.push_back(i);
                positionC.push_back(j);
                Point p = get_intersection(vertexS[i % vertexS.size()], vertexS[(i + 1) % vertexS.size()], 
                    vertexC[j % vertexC.size()], vertexC[(j + 1) % vertexC.size()]);
                intersect_point.push_back(p);
                count = count + 1;
            }
        }
    }
    if (count == 0)
    {
        cout << "Polygons have no points of intersection." << endl;
        return result;
    }
    int startposition = 0;
    int s_c = 0;
    vector<Point> startspoints;
    if (positionS[1] == positionS[0])
    {
        if (distanсe_(intersect_point[0], vertexS[positionS[0]]) < distanсe_(intersect_point[1], vertexS[positionS[1]]))
        {
            result.push_back(intersect_point[0]);
            startposition = 0;
        }
        else
        {
            result.push_back(intersect_point[1]);
            startposition = 1;
        }
    }
    else
    {
        result.push_back(intersect_point[0]);
        startposition = 0;
    }
    result.push_back(vertexS[positionS[startposition]]);
    int currentposition = -1;
    int numbervoit = positionS[startposition];
    numbervoit = get_previous_index_circular(numbervoit, vertexS.size());

    while (currentposition != startposition)
    {
        if (s_c == 0)
        {
            int count = 0;
            vector <Point> intPoint;
            vector <int> positionIntPoint;
            for (int i = 0; i < positionS.size(); i++)
            {
                if (positionS[i] == numbervoit)
                {
                    count = count + 1;
                    intPoint.push_back(intersect_point[i]);
                    positionIntPoint.push_back(i);
                }
            }
            if (count == 0)
            {
                result.push_back(vertexS[numbervoit]);
                numbervoit = get_previous_index_circular(numbervoit, vertexS.size());
            }
            if (count == 1)
            {
                result.push_back(intPoint[0]);
                currentposition = positionIntPoint[0];
                numbervoit = positionC[positionIntPoint[0]];
                result.push_back(vertexC[numbervoit]);
                numbervoit = get_previous_index_circular(numbervoit, vertexC.size());
                s_c = 1;
            }
            if (count == 2)
            {
                if (distanсe_(intPoint[0], vertexS[numbervoit]) > distanсe_(intPoint[1], vertexS[numbervoit]))
                {
                    result.push_back(intPoint[0]);
                    currentposition = positionIntPoint[0];
                    numbervoit = positionC[positionIntPoint[0]];
                    result.push_back(vertexC[numbervoit]);
                    numbervoit = get_previous_index_circular(numbervoit, vertexC.size());
                    s_c = 1;
                }
                else
                {
                    result.push_back(intPoint[1]);
                    currentposition = positionIntPoint[1];
                    numbervoit = positionC[positionIntPoint[1]];
                    result.push_back(vertexC[numbervoit]);
                    numbervoit = get_previous_index_circular(numbervoit, vertexC.size());
                    s_c = 1;
                }
            }
            intPoint.clear();
            positionIntPoint.clear();
        }
        else
        {
            int count = 0;
            vector <Point> intPoint;
            vector <int> positionIntPoint;
            for (int i = 0; i < positionC.size(); i++)
            {
                if (positionC[i] == numbervoit)
                {
                    count = count + 1;
                    intPoint.push_back(intersect_point[i]);
                    positionIntPoint.push_back(i);
                }
            }
            if (count == 0)
            {
                result.push_back(vertexC[numbervoit]);
                numbervoit = get_previous_index_circular(numbervoit, vertexC.size());
            }
            if (count == 1)
            {
                result.push_back(intPoint[0]);
                currentposition = positionIntPoint[0];
                numbervoit = positionS[positionIntPoint[0]];
                result.push_back(vertexS[numbervoit]);
                numbervoit = get_previous_index_circular(numbervoit, vertexS.size());
                s_c = 0;
            }
            if (count == 2)
            {
                if (distanсe_(intPoint[0], vertexC[numbervoit]) > distanсe_(intPoint[1], vertexC[numbervoit]))
                {
                    result.push_back(intPoint[0]);
                    currentposition = positionIntPoint[0];
                    numbervoit = positionS[positionIntPoint[0]];
                    result.push_back(vertexS[numbervoit]);
                    numbervoit = get_previous_index_circular(numbervoit, vertexS.size());
                    s_c = 0;
                }
                else
                {
                    result.push_back(intPoint[1]);
                    currentposition = positionIntPoint[1];
                    numbervoit = positionS[positionIntPoint[1]];
                    result.push_back(vertexS[numbervoit]);
                    numbervoit = get_previous_index_circular(numbervoit, vertexS.size());
                    s_c = 0;
                }
            }
            intPoint.clear();
            positionIntPoint.clear();
        }
    }
    return slice(result, 0, result.size() - 3);
}


Point bezier_points(Point P0, Point P1, Point P2, Point P3, double t) {
    return pow(1 - t, 3) * P0 + 3 * t * pow(1 - t, 2) * P1 + 3 * pow(t, 2) * (1 - t) * P2 + pow(t, 3) * P3;
}

void bezier_curve(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3) {
    float H = max_dist(P0, P1, P2, P3);
    int N = 1 + static_cast<int>(sqrt(3 * H));

    Point prevPoint = P0; // Start with the first point
    for (int i = 1; i <= N; ++i) {
        float t = static_cast<float>(i) / N;
        Point point = bezier_points(P0, P1, P2, P3, t);
        draw_line(img, prevPoint.x, prevPoint.y, point.x, point.y, 0xB4A7D6); // Draw line to the current point
        prevPoint = point; // Update the previous point
    }

    // Draw the control points
    for (auto& point : { P0, P1, P2, P3 }) {
        draw_line(img, point.x - 3, point.y, point.x + 3, point.y, 0x000000);
        draw_line(img, point.x, point.y - 3, point.x, point.y + 3, 0x000000);
    }
}

bool check_smoothness(Point& P2, Point& P3, Point& Q1) {
    float P2P3 = sqrt(distanсe_(P2, P3));
    float P3Q1 = sqrt(distanсe_(P3, Q1));
    float P2Q1 = sqrt(distanсe_(P2, Q1));
    return abs(P2P3 + P3Q1 - P2Q1) < 1e-6;
}

void composite_bezier_curve(Mat& img, vector<Point>& controlPoints) {
    int n = controlPoints.size();
    if (n < 4 || n % 3 != 1) {
        cerr << "Amount of control points must be equal to 4, 7, 10, etc." << endl;
        return;
    }

    for (size_t i = 0; i < n - 3; i += 3) {
        Point P0 = controlPoints[i];
        Point P1 = controlPoints[i + 1];
        Point P2 = controlPoints[i + 2];
        Point P3 = controlPoints[i + 3];


        if (i != n - 4)
            if (!check_smoothness(P2, P3, controlPoints[i + 4])) {
                cerr << "The connection is not smooth, choose another option for point Q1" << endl;
                return;
            }

        bezier_curve(img, P0, P1, P2, P3);
    }
}

void RGBtoHSL(float r, float g, float b, float& h, float& s, float& l) {
    r /= 255.0;
    g /= 255.0;
    b /= 255.0;

    float maxVal = max(r, max(g, b));
    float minVal = min(r, min(g, b));
    float delta = maxVal - minVal;

    // Luminance
    l = (maxVal + minVal) / 2.0;

    // Saturation
    if (delta == 0) {
        s = 0;
        h = 0; //для гистограмм вместо NULL
        l = 0;
        return;
    }

    if (l == 0) {
        s = 0.0;
    }
    else if (l < 0.5) {
        s = delta / (maxVal + minVal);
    }
    else {
        if (2.0 - maxVal - minVal == 0.0) {
            s = 0;
        }
        else {
            s = delta / (2.0 - maxVal - minVal);
        }
    }

    // Hue
    if (r == maxVal) {
        h = (g - b) / delta;
    }
    else if (g == maxVal) {
        h = 2.0 + (b - r) / delta;
    }
    else {
        h = 4.0 + (r - g) / delta;
    }

    h *= 60.0;
    if (h < 0) {
        h += 360.0;
    }
}
