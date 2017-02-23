#include "Point.h"

#include <iostream>
#include <fstream>
#include <iomanip>

Point::Point(double x_coord, double y_coord) :
        x(x_coord),
        y(y_coord)
{}

Point::Point(const Point &vec2_to_copy) :
        x(vec2_to_copy.x),
        y(vec2_to_copy.y)
{}

Point Point::operator=(const Point & other_vec)
{
    this->x = other_vec.x;
    this->y = other_vec.y;
    return *this;
}

void Point::save(const char * filename)
{
    std::ofstream file(filename, std::ios_base::app);
    file << x << " " << y << std::endl;
}