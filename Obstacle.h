#ifndef GD_PROJECT_OBSTACLE_H
#define GD_PROJECT_OBSTACLE_H

#include "Point.h"

struct Obstacle {
    Point min_vec;
    Point max_vec;
    double diagonal;

    Obstacle(Point other_min, Point other_max);
    void save(const char * filename);
};


#endif //GD_PROJECT_OBSTACLE_H
