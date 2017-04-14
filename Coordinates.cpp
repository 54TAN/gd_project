#include "Coordinates.h"

#include <iostream>
#include <fstream>
#include <iomanip>

Coordinates::Coordinates(double x_, double y_, double phi_, double length_) :
        x(x_),
        y(y_),
        phi(phi_),
        length(length_)
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

Coordinates::Coordinates(double x_, double y_) :
        x(x_),
        y(y_)
{
    coords.push_back(x);
    coords.push_back(y);
    phi = 0;
    coords.push_back(0);
}

bool Coordinates::operator==(Coordinates other) 
{
    return (coords == other.coords) ? true : false;
}

bool Coordinates::operator!=(Coordinates other)
{
    return !(*this == other);
}
