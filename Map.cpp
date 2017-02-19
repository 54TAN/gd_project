#include "Map.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

void Map::generate_obstacles(int num, int width, int height, double min_diag, double max_diag)
{
    for (int i = 0; i < num; i++) {
        obstacles.push_back(gen_rand_obstacle(width, height, min_diag, max_diag));
    }
    this->width = width;
    this->height = height;
}

void Map::generate_points(int num, int width, int height)
{
    if (!obstacles.size()){
        for (size_t i = 0; i < num; i++) {
            points.push_back(gen_GeomVector(width, height, 0, 0));
        }
    } else {
        int k = 0;
        while (k != num) {
            GeomVector new_point = gen_GeomVector(width, height);
            if (is_point_in_obstacle(new_point)) {
                points.push_back(new_point);
                k++;
            }
        }
    }
}

GeomVector Map::gen_GeomVector(int width, int height, int min_x, int min_y)
{
    //srand(time(NULL));

    int x_r = rand() % (width - min_x) + min_x;
    int y_r = rand() % (height - min_y) + min_y;

    GeomVector test_point(x_r, y_r);

    if (is_point_in_obstacle(test_point) && !among_points(test_point)) {
        return test_point;
    } else {
        return gen_GeomVector(width, height, min_x, min_y);
    }

}

Obstacle Map::gen_rand_obstacle(int width, int height, double min_diag, double max_diag)
{
    GeomVector new_min = gen_GeomVector(width, height);
    GeomVector new_max = gen_GeomVector(width, height, new_min.x, new_min.y);

    if ((new_max.x - new_min.x < 50) || (new_max.y - new_min.y < 50)) {
        return gen_rand_obstacle(width, height, min_diag, max_diag);
    }

    Obstacle test_Obstacle(new_min, new_max);

    if (test_Obstacle.diagonal > min_diag && test_Obstacle.diagonal < max_diag
        && obstacle_intersection(test_Obstacle)) {
        //std::cout << "Obstacle" << std::endl;
        return test_Obstacle;
    } else {
        return gen_rand_obstacle(width, height, min_diag, max_diag);
    }

}

void Map::save(const char * file_for_obstacles, const char * file_for_points)
{
    if (obstacles.size()){
        std::ofstream file(file_for_obstacles, std::ios_base::trunc);
        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i].save(file_for_obstacles);
        }
    }
    if (points.size()) {
        std::ofstream file(file_for_points, std::ios_base::trunc);
        for (size_t i = 0; i < points.size(); i++) {
            points[i].save(file_for_points);
        }
    }
}

bool Map::is_point_in_obstacle(GeomVector point)
{
    for (size_t i = 0; i < obstacles.size(); i++) {
        if (obstacles[i].min_vec.x < point.x &&
            obstacles[i].max_vec.x > point.x &&
            obstacles[i].min_vec.y < point.y &&
            obstacles[i].max_vec.y > point.y) {
            return false;
        }
    }
    return true;
}

bool Map::check_line(GeomVector point_1, GeomVector point_2)
{
    bool x_y;
    if (point_1.x == point_2.x) { x_y = 1;}
    if (point_1.y == point_2.y) { x_y = 0;}

    int const_coord = (x_y) ? point_1.x : point_1.y;
    int mov_coord = (x_y) ? std::min(point_1.y, point_2.y) :
                    std::min(point_1.x, point_2.x);
    int coord_fin = (x_y) ? std::max(point_1.y, point_2.y) :
                    std::max(point_1.x, point_2.x);
    //std::cout << "/* message *//////////////////////////////////" << std::endl;
    for (size_t i = 0; mov_coord != coord_fin; i++) {
        GeomVector temp_point = (x_y) ? GeomVector(const_coord, mov_coord) :
                          GeomVector(mov_coord, const_coord);
        if (!is_point_in_obstacle(temp_point)) {
            return false;
        } else {
            mov_coord++;
        }
    }

    return true;

}

bool Map::obstacle_intersection(Obstacle new_one) // как по-другому, я не придумал
{
    if (check_line(new_one.min_vec, GeomVector(new_one.min_vec.x, new_one.max_vec.y)) &&
        check_line(new_one.min_vec, GeomVector(new_one.max_vec.x, new_one.min_vec.y)) &&
        check_line(GeomVector(new_one.min_vec.x, new_one.max_vec.y), new_one.max_vec) &&
        check_line(GeomVector(new_one.max_vec.x, new_one.min_vec.y), new_one.max_vec)){
        return true;
    }

    return false;
}

bool Map::among_points(GeomVector point)
{
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].x == point.x && points[i].y == point.y) { return true; }
    }

    return false;
}
