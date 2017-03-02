#ifndef GD_PROJECT_OBSTACLE_H
#define GD_PROJECT_OBSTACLE_H

#include "Coordinates.h"

struct Obstacle {
    Coordinates min_vec;
    Coordinates max_vec;
    double diagonal;

    Obstacle(Coordinates other_min, Coordinates other_max);
    void save(const char * filename);
};


#endif //GD_PROJECT_OBSTACLE_H
