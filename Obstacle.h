#ifndef GD_PROJECT_OBSTACLE_H
#define GD_PROJECT_OBSTACLE_H

#include "GeomVector.h"

struct Obstacle {
    GeomVector min_vec;
    GeomVector max_vec;
    double diagonal;

    Obstacle(GeomVector other_min, GeomVector other_max);
    void save(const char * filename);
};


#endif //GD_PROJECT_OBSTACLE_H
