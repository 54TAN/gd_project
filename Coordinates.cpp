#include "Coordinates.h"

#include <iostream>
#include <fstream>
#include <iomanip>

Coordinates::Coordinates(double x_coord, double y_coord, double phi, double length) :
        x(x_coord),
        y(y_coord),
        phi(phi),
        length(length)
{
    coords.push_back(x);
    coords.push_back(y);
    //coords.push_back(phi);
}

Coordinates::Coordinates(Coordinates const& other_vec) :
        x(other_vec.x),
        y(other_vec.y),
        phi(other_vec.phi),
        length(other_vec.length)
{
    coords = other_vec.coords;
}

Coordinates& Coordinates::operator=(Coordinates const & other_vec)
{
    x = other_vec.x;
    y = other_vec.y;
    phi = other_vec.phi;
    length = other_vec.length;
    coords = other_vec.coords;
    return *this;
}

void Coordinates::save(const char * filename)
{
    std::ofstream file(filename, std::ios_base::app);
    file << x << " " << y << std::endl;
}