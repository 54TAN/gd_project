#ifndef GD_PROJECT_MAP_H
#define GD_PROJECT_MAP_H

#include <vector>
#include "Coordinates.h"
#include "Obstacle.h"

struct Map {
    std::vector<Obstacle> obstacles;
    std::vector<Coordinates> points;
    int width;
    int height;

    void generate_obstacles(int num, int width, int height, double min_diag, double max_diag);
    void generate_points(int num, int width, int height);
    void save(const char * file_for_Obstacles, const char * file_for_points);

    bool is_point_in_obstacle(Coordinates point);
    bool among_points(Coordinates point);
    bool obstacle_intersection(Obstacle new_one);
    bool check_line(Coordinates point_1, Coordinates point_2);

    Coordinates gen_Point(int width, int height, int min_x = 0, int min_y = 0);
    Obstacle gen_rand_obstacle(int width, int height, double min_diag, double max_diag);
};


#endif //GD_PROJECT_MAP_H
