#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv2/opencv.hpp> 
#include <iostream> 

using namespace cv;
using namespace std;


void set_pixel(Mat& img, int x, int y, int color) {
    int r = color & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = (color >> 16) & 0xFF;
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
        img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}

void draw_line(Mat& img, int x1, int y1, int x2, int y2, int color); //task1
void draw_polygon(Mat& img, vector<Point> points, int color);    //task2

//task3:
double get_angle(Point p1, Point p2, Point p3);
bool is_convex(vector<Point> points);
bool intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
bool is_simple(vector<Point> points);

//task4:
bool inside_eo(const vector<Point>& points, const Point& p);
bool inside_nzw(const vector<Point>& points, const Point& p);
void fill_polygon(Mat& img, const vector<Point>& points, int color, bool useEvenOddRule);


int main(int argc, char** argv)
{
    //task 1

    /*
    Mat img(500, 500, CV_8UC3, Scalar(255, 255, 255));

    draw_line(img, 10, 10, 300, 440, 0x440154);
    draw_line(img, 64, 10, 420, 500, 0x480C8B);
    draw_line(img, 0, 0, 18, 4, 0x21918C);
    draw_line(img, 108, 8, 108, 400, 0x7FCD57);

    imshow("lines", img);
    waitKey(0);


    imwrite("lines.png", img);
    */

    //task 2
    /*
    Mat img1(500, 500, CV_8UC3, Scalar(255, 255, 255));
    Mat img2(500, 500, CV_8UC3, Scalar(255, 255, 255));
    Mat img3(500, 500, CV_8UC3, Scalar(255, 255, 255));

    vector<Point> points1 = {Point(50, 50), Point(450, 50), Point(450, 450), Point(50, 450)};
    vector<Point> points2 = {Point(250, 50), Point(375, 400), Point(50, 150), Point(450, 150), Point(125, 400)};
    vector<Point> points3 = { Point(300, 90), Point(72, 360), Point(450, 60), Point(420, 290), Point(301, 499), Point(88, 132) };

    draw_polygon(img1, points1, 0x440154);
    draw_polygon(img2, points2, 0x480C8B);
    draw_polygon(img3, points3, 0x21918C);

    waitKey(0);

    imwrite("square.png", img1);
    imwrite("star.png", img2); 
    imwrite("poly.png", img3);
    */

    //task 3
    /*
    vector<Point> points1 = { Point(50, 50), Point(450, 50), Point(450, 450), Point(50, 450) };
    vector<Point> points2 = { Point(250, 50), Point(375, 400), Point(50, 150), Point(450, 150), Point(125, 400) };

    cout << (is_convex(points1) ? "True" : "False") << endl;
    //Выводит true
    cout << (is_simple(points1) ? "True" : "False") << endl;
    //Выводит true
    cout << (is_convex(points2) ? "True" : "False") << endl;
    //Выводит false
    cout << (is_simple(points2) ? "True" : "False") << endl;
    //Выводит false
    */
    
    //task 4
    Mat img1(500, 500, CV_8UC3, Scalar(255, 255, 255));
    Mat img2(500, 500, CV_8UC3, Scalar(255, 255, 255));
    Mat img3(500, 500, CV_8UC3, Scalar(255, 255, 255));

    vector<Point> points1 = { Point(50, 50), Point(450, 50), Point(450, 450), Point(50, 450) };
    vector<Point> points2 = { Point(250, 50), Point(375, 400), Point(50, 150), Point(450, 150), Point(125, 400) };
    vector<Point> points3 = { Point(300, 90), Point(72, 360), Point(450, 60), Point(420, 290), Point(301, 499), Point(88, 132) };

    draw_polygon(img1, points1, 0x440154);
    draw_polygon(img2, points2, 0x480C8B);
    draw_polygon(img3, points3, 0x21918C);

    /*
    fill_polygon(img1, points1, 0x440154, false);
    fill_polygon(img2, points2, 0x480C8B, false);
    fill_polygon(img3, points3, 0x21918C, false);

    imwrite("square_nzw.png", img1);
    imwrite("star_nzw.png", img2);
    imwrite("poly_nzw.png", img3);
    */

    fill_polygon(img1, points1, 0x440154, true);
    fill_polygon(img2, points2, 0x480C8B, true);
    fill_polygon(img3, points3, 0x21918C, true);

    imwrite("square_eo.png", img1);
    imwrite("star_eo.png", img2);
    imwrite("poly_eo.png", img3);

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

void draw_polygon(Mat& img, vector<Point> points, int color) {
    for (int i = 0; i < points.size(); i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];
        draw_line(img, p1.x, p1.y, p2.x, p2.y, color);
    }
}

bool intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
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
        if (intersect(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y,
            points[i + 2].x, points[i + 2].y, points[i + 3].x, points[i + 3].y)) {
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

bool inside_eo(const vector<Point>& points, const Point& p) {
    int count = 0;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];

        if ((p1.y > p.y) != (p2.y > p.y)) {
            double xIntersection = (double)(p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y) + p1.x;
            if (p.x < xIntersection) {
                count++;
            }
        }
    }
    return (count % 2) == 1;
}

bool inside_nzw(const vector<Point>& points, const Point& p) {
    int windingNumber = 0;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];

        if (p1.y <= p.y) {
            if (p2.y > p.y && (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x) > 0) {
                windingNumber++;
            }
        }
        else {
            if (p2.y <= p.y && (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x) < 0) {
                windingNumber--;
            }
        }
    }
    return windingNumber != 0;
}

void fill_polygon(Mat& img, const vector<Point>& points, int color, bool useEvenOddRule) {
    int minX = img.cols, maxX = 0, minY = img.rows, maxY = 0;
    for (const Point& p : points) {
        minX = min(minX, p.x);
        maxX = max(maxX, p.x);
        minY = min(minY, p.y);
        maxY = max(maxY, p.y);
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            Point p(x, y);
            bool inside = useEvenOddRule ? inside_eo(points, p) : inside_nzw(points, p);
            if (inside) {
                int r = color & 0xFF;
                int g = (color >> 8) & 0xFF;
                int b = (color >> 16) & 0xFF;
                set_pixel(img, x, y, color);
            }
        }
    }
}
