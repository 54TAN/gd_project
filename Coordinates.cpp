#include "Coordinates.h"

#include <iostream>
#include <fstream>
#include <iomanip>

Coordinates::Coordinates(double x_coord, double y_coord, double phi) :
        x(x_coord),
        y(y_coord),
        phi(phi)
{
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(phi);
}

Coordinates::Coordinates(const Coordinates &other_vec) :
        x(other_vec.x),
        y(other_vec.y),
        phi(other_vec.phi)
{}

Coordinates Coordinates::operator=(const Coordinates & other_vec)
{
    x = other_vec.x;
    y = other_vec.y;
    phi = other_vec.phi;
    coords = other_vec.coords;
    return *this;
}

void Coordinates::save(const char * filename)
{
    std::ofstream file(filename, std::ios_base::app);
    file << x << " " << y << std::endl;
}