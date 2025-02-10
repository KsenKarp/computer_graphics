#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;


//from previous labs:
void set_pixel(Mat& img, int x, int y, int color) {
    int r = color & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = (color >> 16) & 0xFF;
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
        img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}
void draw_line(Mat& img, int x1, int y1, int x2, int y2, int color);

//task1:
void draw_projection(Mat& img, const vector<Point3f>& vertices, const Mat& transformation_matrix);
Mat orthographic_projection(double n);
Mat axonometric_projection(double phi, double theta);
Mat oblique_projection(double h, double i);

//task2:
Mat perspective_projection(double k);

//task3:
void draw_visible_faces(Mat& img, const vector<Point3f>& vertices, const Mat& transformation_matrix);

//task4:
Mat rotation_matrix(double angle, double x, double y, double z);


int main(int argc, char** argv)
{

    Mat img(500, 500, CV_8UC3, Scalar(255, 255, 255));
    vector<Point3f> cube = {
        Point3f(-50, -50, 50),
        Point3f(-50, -50, -50),
        Point3f(-50, -150, 50),
        Point3f(-50, -150, -50),
        Point3f(-150, -50, 50),
        Point3f(-150, -50, -50),
        Point3f(-150, -150, 50),
        Point3f(-150, -150, -50)
    };
    

    //task1
    // ортографическая проекция
    /*
    Mat orthographic_matrix = orthographic_projection(0.0);
    draw_projection(img, cube, orthographic_matrix);
    imshow("Ortho_projection", img);
    waitKey(0);
    imwrite("Ortho_projection.png", img);
    */

    // кабинетная проекция
    /*
    Mat cabinet_matrix = oblique_projection(0.5 * cos(M_PI / 4), 0.5 * cos(M_PI / 4));
    draw_projection(img, cube, cabinet_matrix);
    imshow("Cabinet_projection", img);
    waitKey(0);
    imwrite("Cabinet_projection.png", img);
    */

    // свободная проекция
    /*
    Mat cavalier_matrix = oblique_projection(cos(M_PI / 4), cos(M_PI / 4));
    draw_projection(img, cube, cavalier_matrix);
    imshow("Oblique_projection", img);
    waitKey(0);
    imwrite("Oblique_projection.png", img);
    */

    // изометричесая проекция
    /*
    Mat isometric_matrix = axonometric_projection(45, 32);
    draw_projection(img, cube, isometric_matrix);
    imshow("Isometric_projection", img);
    waitKey(0);
    imwrite("Isometric_projection.png", img);
    */

    // диаметрическая проекция
    /*
    Mat img(500, 1000, CV_8UC3, Scalar(255, 255, 255));
    Mat diametric_mtrix = axonometric_projection(90, 45);
    draw_projection(img, cube, diametric_mtrix);
    imshow("Diametric_projection", img);
    waitKey(0);
    imwrite("Diametric_projection.png", img);
    */


    //task2
    /*
    Mat perspective_projection_matrix = perspective_projection(0.0025);
    draw_projection(img, cube, perspective_projection_matrix);
    imshow("Perspective_projection", img);
    waitKey(0);
    imwrite("Perspective_projection.png", img);
    */


    //task3
    // свободная проекция
    /*
    Mat cavalier_matrix = oblique_projection(cos(M_PI / 4), cos(M_PI / 4));
    draw_visible_faces(img, cube, cavalier_matrix);
    imshow("Cavalier_visible_only", img);
    waitKey(0);
    imwrite("Cavalier_visible_only.png", img);
    */

    // кабинетная проекция
    /*
    Mat cabinet_matrix = oblique_projection(0.5 * cos(M_PI / 4), 0.5 * cos(M_PI / 4));
    draw_visible_faces(img, cube, cabinet_matrix);
    imshow("Cabinet_visible_only", img);
    waitKey(0);
    imwrite("Cabinet_visible_only.png", img);
    */


    //task4
    
    VideoWriter video("rotating_parallelepiped_perspective.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(500, 500));
    double angle = 0.0f;
    double rotationAxisX = 2.5f, rotationAxisY = 2.5f, rotationAxisZ = 0.5f;

    while (true) {
        img.setTo(Scalar(255, 255, 255));
        Mat rot_matrix = rotation_matrix(angle, rotationAxisX, rotationAxisY, rotationAxisZ);
        Mat perspective_matrix = perspective_projection(-0.001);
        draw_visible_faces(img, cube, rot_matrix * perspective_matrix);

        video.write(img);
        imshow("Rotating cube", img);
        angle += 0.01f;
        if (waitKey(10) >= 0) break;
    }

    video.release();
    
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

void draw_projection(Mat& img, const vector<Point3f>& vertices, const Mat& transformation_matrix) {
    vector<Point2f> projected_points;

    // apply matrix foe each vertice of a parralelipiped
    for (const auto& vertex : vertices) {
        Mat point = (Mat_<double>(4, 1) << vertex.x, vertex.y, vertex.z, 1);
        Mat transformed_point = transformation_matrix.t() * point;

        // normalizing
        double H = transformed_point.at<double>(3);
        Point2f projected_point(transformed_point.at<double>(0) / H, transformed_point.at<double>(1) / H);
        projected_points.push_back(projected_point);
    }

    vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, 
        {0, 4}, {1, 3}, 
        {1, 5}, {2, 3}, 
        {2, 6}, {3, 7},
        {4, 5}, {4, 6},
        {5, 7}, {6, 7}
    };

    for (const auto& edge : edges) {
        draw_line(img, projected_points[edge.first].x + 350, projected_points[edge.first].y + 350,
            projected_points[edge.second].x + 350, projected_points[edge.second].y + 350, 0xB4A7D6);
    }
}

Mat orthographic_projection(double n) { //projection on z=n
    return (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, n, 1);
}

Mat axonometric_projection(double phi, double theta) {
    Mat first = (Mat_<double>(4, 4) <<
        cos(phi), 0, -sin(phi), 0,
        0, 1, 0, 0,
        sin(phi), 0, cos(phi), 0,
        0, 0, 0, 1);

    Mat second = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, cos(theta), sin(theta), 0,
        0, -sin(theta), cos(theta), 0,
        0, 0, 0, 1);

    return  first * second * orthographic_projection(0); //on z=0
}

Mat oblique_projection(double h, double i) {
    Mat first = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        h, i, 1, 0,
        0, 0, 0, 1);

    return  first * orthographic_projection(0); //on z=0
}

// one-point perspective, center in [0, 0, k]
Mat perspective_projection(double k) {
    return (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, k,
        0, 0, 0, 1);
}

void draw_visible_faces(Mat& img, const vector<Point3f>& vertices, const Mat& transformation_matrix) {
    vector<Point2f> projected_points;

    // apply matrix
    for (const auto& vertex : vertices) {
        Mat point = (Mat_<double>(4, 1) << vertex.x, vertex.y, vertex.z, 1);
        Mat transformed_point = transformation_matrix.t() * point;

        // normalizing
        double H = transformed_point.at<double>(3);
        Point2f projected_point(transformed_point.at<double>(0) / H, transformed_point.at<double>(1) / H);
        projected_points.push_back(projected_point);
    }

    // find faces and normal vectors pointing inside the parrallepiped -> using right-hand rule we find vertices that belong to each face
    vector<vector<int>> faces = {
        {2, 3, 7, 6},
        {0, 4, 5, 1},
        {4, 6, 7, 5},
        {0, 1, 3, 2},
        {1, 5, 7, 3},
        {0, 2, 6, 4},
    };

    // back face culling:
    Vec3f v(0, 0, -1);

    for (const auto& face : faces) {
        // find normal vertice for each face
        Vec3f ab = Vec3f(projected_points[face[1]].x - projected_points[face[0]].x,
            projected_points[face[1]].y - projected_points[face[0]].y, 0);
        Vec3f ac = Vec3f(projected_points[face[2]].x - projected_points[face[0]].x,
            projected_points[face[2]].y - projected_points[face[0]].y, 0);
        Vec3f normal = ab.cross(ac);
        normal = normal / norm(normal);

        // check visibility
        if (normal.dot(v) > 0) {
            for (size_t i = 0; i < face.size(); ++i) {
                draw_line(img, projected_points[face[i]].x + 350, projected_points[face[i]].y + 350,
                    projected_points[face[(i + 1) % face.size()]].x + 350, projected_points[face[(i + 1) % face.size()]].y + 350, 0xB4A7D6);
            }
        }
    }
}

Mat rotation_matrix(double angle, double x, double y, double z) {
    double n = sqrt(x * x + y * y + z * z);
    double nx = x / n;
    double ny = y / n;
    double nz = z / n;

    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    return (Mat_<double>(4, 4) <<
        cos_angle + nx * nx * (1 - cos_angle), nx * ny * (1 - cos_angle) + nz * sin_angle, nx * nz * (1 - cos_angle) - ny * sin_angle, 0,
        ny * nx * (1 - cos_angle) - nz * sin_angle, cos_angle + ny * ny * (1 - cos_angle), ny * nz * (1 - cos_angle) + nx * sin_angle, 0,
        nz * nx * (1 - cos_angle) + ny * sin_angle, nz * ny * (1 - cos_angle) - nx * sin_angle, cos_angle + nz * nz * (1 - cos_angle), 0,
        0, 0, 0, 1);
}
