#pragma once
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp> 
#include <iostream> 
#include <cmath>

using namespace cv;
using namespace std;

//from previous lab:
void set_pixel(Mat& img, int x, int y, int color) {
    int r = color & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = (color >> 16) & 0xFF;
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
        img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}
void draw_line(Mat& img, int x1, int y1, int x2, int y2, int color);
void draw_polygon(Mat& img, vector<Point> points, int color);
double get_angle(Point p1, Point p2, Point p3);
bool is_convex(vector<Point> points);
bool intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
bool is_simple(vector<Point> points);

//task1:
float distance(Point P0, Point P1) {
    return abs(P0.x - P1.x) + abs(P0.y - P1.y);
}

float max_dist(Point P0, Point P1, Point P2, Point P3) {
    float d1 = distance(P0 - 2 * P1 + P2, Point(0, 0));
    float d2 = distance(P1 - 2 * P2 + P3, Point(0, 0));
    return max(d1, d2);
}

Point bezier_points(Point P0, Point P1, Point P2, Point P3, double t);
void bezier_curve(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3);

//task2:
double polygon_area(const vector<Point>& points);
bool clockwise(const vector<Point>& points);
void cyrus_beck_algorithm(Mat& img, Point& p1, Point& p2, const vector<Point>& points);



//additional task
void bezier_curve_n(Mat& img, const vector<Point>& controlPoints);
int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}
double bernstein(int n, int i, double t) {
    return static_cast<double>(factorial(n)) / (factorial(i) * factorial(n - i)) * pow(t, i) * pow(1 - t, n - i);
}
float max_dist_n(const vector<Point>& controlPoints) {
    float maxDistance = 0.0f;
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        for (size_t j = i + 1; j < controlPoints.size(); ++j) {
            float dist = distance(controlPoints[i], controlPoints[j]);
            if (dist > maxDistance) {
                maxDistance = dist;
            }
        }
    }
    return maxDistance;
}

int main(int argc, char** argv)
{

    Mat img(500, 500, CV_8UC3, Scalar(255, 255, 255));

    vector<Point> Points1 = {
        {100, 300},
        {70, 450},
        {50, 100},
        {240, 100}
    };
    vector<Point> Points2 = {
        {250, 250}, 
        {150, 150}
    };
    vector<Point> Points3 = {
        {70, 450},
        {100, 300},
        {400, 200},
        {250, 250},
        {50, 100},
        {240, 100},
        {150, 150},
        {250, 200}
    };
    

    bezier_curve_n(img, Points1);
    bezier_curve_n(img, Points2);
    bezier_curve_n(img, Points3);
    
    /*
    Point p1 = { 100, 300 };
    Point p2 = { 70, 450 };
    Point p3 = { 50, 100 };
    Point p4 = { 240, 100 };
    */

    //bezier_curve(img, p1, p2, p3, p4);
    imshow("Bezier Curve", img);
    waitKey(0);
    imwrite("bezier_n.png", img);


    //task 2
    /*
    Mat img(500, 500, CV_8UC3, Scalar(255, 255, 255));
    vector<Point> Points = {
        Point(50, 200),
        Point(250, 300),
        Point(350, 250),
        Point(300, 100),
        Point(250, 50),
        Point(100, 100)
    };
    Point p1 = { 100, 300 };
    Point p2 = { 70, 450 };
    Point p3 = { 50, 100 };
    Point p4 = { 240, 100 };
    Point p5 = { 400, 200 };
    Point p6 = { 250, 250 };
    Point p7 = { 150, 150 };
    Point p8 = { 250, 200 };
    cyrus_beck_algorithm(img, p1, p2, Points);
    cyrus_beck_algorithm(img, p3, p4, Points);
    cyrus_beck_algorithm(img, p5, p6, Points);
    cyrus_beck_algorithm(img, p7, p8, Points);

    imshow("cyrus_beck", img);
    waitKey(0);
    imwrite("cyrus_beck.png", img);
    */

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

void bezier_curve_n(Mat& img, const vector<Point>& controlPoints) {
    if (controlPoints.size() < 2) {
        cout << "not enough points!" << endl;
        return;
    }
    int n = controlPoints.size() - 1;
    float H = max_dist_n(controlPoints);
    int N = 1 + static_cast<int>(sqrt(3 * H));

    Point prevPoint = controlPoints[0]; //start with the first control point
    for (int i = 1; i <= N; ++i) {
        float t = static_cast<float>(i) / N;
        Point point(0, 0);

        for (int j = 0; j <= n; ++j) {
            double b = bernstein(n, j, t);
            point += b * controlPoints[j];
        }

        draw_line(img, prevPoint.x, prevPoint.y, point.x, point.y, 0xB4A7D6);
        prevPoint = point;
    }

    for (const auto& point : controlPoints) {
        draw_line(img, point.x - 3, point.y, point.x + 3, point.y, 0x000000);
        draw_line(img, point.x, point.y - 3, point.x, point.y + 3, 0x000000);
    }
}

double polygon_area(const vector<Point>& points) {
    double area = 0.0;
    int n = points.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += points[i].x * points[j].y;
        area -= points[j].x * points[i].y;
    }
    return area / 2.0;
}

bool clockwise(const vector<Point>& points) {
    double area = polygon_area(points);
    return area < 0;
}

void cyrus_beck_algorithm(Mat& img, Point& p1, Point& p2, const vector<Point>& points) {

    // checking for the convexity of the polygon
    if (!is_convex(points)) {
        cerr << "Polygon is not convex!" << endl;
        return;
    }
    // checking for the correct orientation of the polygon
    if (!clockwise(points)) {
        cerr << "Polygon is not correctly oriented!" << endl;
        return;
    }

    draw_polygon(img, points, 0x000000);
    draw_line(img, p1.x, p1.y, p2.x, p2.y, 0x000000);

    int n = points.size();
    double t1 = 0, t2 = 1, t;
    double sx = p2.x - p1.x, sy = p2.y - p1.y;
    double nx, ny, denom, num;

    for (int i = 0; i < n; i++) {
        nx = points[(i + 1) % n].y - points[i].y;
        ny = points[i].x - points[(i + 1) % n].x;
        denom = nx * sx + ny * sy;
        num = nx * (p1.x - points[i].x) + ny * (p1.y - points[i].y);

        if (denom != 0) {
            t = -num / denom;
            if (denom > 0) {
                if (t > t1) t1 = t;
            }
            else {
                if (t < t2) t2 = t;
            }
        }
        if (t1 > t2) return;
    }

    if (t1 <= t2) {
        Point p1_new = p1 + t1 * (p2 - p1);
        Point p2_new = p1 + t2 * (p2 - p1);

        draw_line(img, p1_new.x, p1_new.y, p2_new.x, p2_new.y, 0x7FCD57);

        p1 = p1_new;
        p2 = p2_new;
    }
}
