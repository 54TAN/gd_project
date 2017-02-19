#include "Obstacle.h"

#include <cmath>

Obstacle::Obstacle(GeomVector other_min, GeomVector other_max) :
        min_vec(other_min),
        max_vec(other_max)
{
    double x_ = other_max.x - other_min.x;
    double y_ = other_max.y - other_min.y;
    diagonal = sqrt(x_*x_ + y_*y_);
}

void Obstacle::save(const char * filename)
{
    min_vec.save(filename);
    max_vec.save(filename);
}
