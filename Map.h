#ifndef GD_PROJECT_MAP_H
#define GD_PROJECT_MAP_H

#include <vector>
#include "GeomVector.h"
#include "Obstacle.h"

struct Map {
    std::vector<Obstacle> obstacles;
    std::vector<GeomVector> points;
    int width;
    int height;

    void generate_obstacles(int num, int width, int height, double min_diag, double max_diag);
    void generate_points(int num, int width, int height);
    void save(const char * file_for_Obstacles, const char * file_for_points);

    bool is_point_in_obstacle(GeomVector point);
    bool among_points(GeomVector point);
    bool obstacle_intersection(Obstacle new_one);
    bool check_line(GeomVector point_1, GeomVector point_2);

    GeomVector gen_GeomVector(int width, int height, int min_x = 0, int min_y = 0);
    Obstacle gen_rand_obstacle(int width, int height, double min_diag, double max_diag);
};


#endif //GD_PROJECT_MAP_H
