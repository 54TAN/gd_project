#include "GeomVector.h"

#include <iostream>
#include <fstream>
#include <iomanip>

GeomVector::GeomVector(double x_coord, double y_coord) :
        x(x_coord),
        y(y_coord)
{}

GeomVector::GeomVector(const GeomVector &vec2_to_copy) :
        x(vec2_to_copy.x),
        y(vec2_to_copy.y)
{}

GeomVector GeomVector::operator=(const GeomVector & other_vec)
{
    this->x = other_vec.x;
    this->y = other_vec.y;
    return *this;
}

void GeomVector::save(const char * filename)
{
    std::ofstream file(filename, std::ios_base::app);
    file << x << " " << y << std::endl;
}