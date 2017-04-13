#include "Geometry.h"

#include <cmath>

void Geometry::get_equation(double * coefs, Coordinates one, Coordinates two) 
{
    coefs[0] = two.y - one.y;
    if (coefs[0] == 0) {
        coefs[1] = 1;
        coefs[2] = -1*two.y;
        return;
    }
    coefs[1] = two.x - one.x;
    if (coefs[1] == 0) {
        coefs[0] = 1;
        coefs[2] = -1*two.x;
        return;
    }
    coefs[2] = (-1 * one.x) * coefs[0] + one.y * coefs[1];
    coefs[1] *= -1;
}

bool Geometry::get_intersection(double * firstLineCoefs, double * secondLineCoefs, double * x) 
{
    double det = firstLineCoefs[0] * secondLineCoefs[1] - firstLineCoefs[1] * secondLineCoefs[0];
    double det_1 = (-1)*firstLineCoefs[2] * secondLineCoefs[1] - firstLineCoefs[1] * (-1)*secondLineCoefs[2];
    double det_2 = firstLineCoefs[0] * (-1)*secondLineCoefs[2] - (-1)*firstLineCoefs[2] * secondLineCoefs[0];

    if (det) {
        *x = det_1/det;
        return true;
    } else {

        if (!det_1 && !det_2) {
            *x = -1*secondLineCoefs[2];
            return true;
        } else {
            return false;
        }

    }
}

void Geometry::up_right_left(int &first, int &second, int intersections, int i, int end_left_y, int end_right_y, bool right) {
    if (intersections == 3) {
        if (i < std::min(end_left_y, end_right_y)) {
            first   = (right) ? 0 : 1;
            second  = (right) ? 1 : 2;
        } else {
            first   = (right) ? 1 : 0;
            second  = (right) ? 2 : 1;
        }
    } else {
        first   = (intersections == 2) ? 0 : 1;
        second  = (intersections == 2) ? 1 : 2;
    }
}

bool Geometry::get_intersection_circle(double * coords, Coordinates circle, std::pair <Coordinates *, Coordinates *> pair) {

    double k = coords[0] / (-1*coords[1]);
    double b = coords[2] / (-1*coords[1]);

    double d = pow(2 * k * b - 2 * circle.x - 2 * circle.y * k,2)
               - (4 + 4 * k * k) * (b * b - pow(circle.length, 2) + pow(circle.x, 2) + pow(circle.y, 2) - 2 * circle.y * b);

    if (d < 0) {
        pair.first->x = -1;
        pair.first->y = -1;
        pair.second->x = -1;
        pair.second->y = -1;
        //std::cout << "none\n";
        return false;
    }

    double x_1 = -1 * ((2 * k * b - 2 * circle.x - 2 * circle.y * k) - sqrt(d)) / (2 + 2 * k * k);
    double x_2 = -1 * ((2 * k * b - 2 * circle.x - 2 * circle.y * k) + sqrt(d)) / (2 + 2 * k * k);


    if (x_1 == x_2) {
        double y = k * x_1 + b;

        pair.first->x = x_1;
        pair.first->y = y;
        pair.second->x = x_1;
        pair.second->y = y;

        //std::cout << "one\n";
        return true;

    }

    double y_1 = k * x_1 + b;
    double y_2 = k * x_2 + b;

    pair.first->x = x_1;
    pair.first->y = y_1;
    pair.second->x = x_2;
    pair.second->y = y_2;

    //std::cout << "two\n";

    return true;
}









