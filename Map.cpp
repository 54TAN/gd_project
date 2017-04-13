#include "Map.h"
#include "Render.h"
#include "Geometry.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
/*

void Map::generate_obstacles(int num, int width, int height, double min_diag, double max_diag) {
    for (int i = 0; i < num; i++) {
        obstacles.push_back(gen_rand_obstacle(width, height, min_diag, max_diag));
    }
    this->width = width;
    this->height = height;
}
*/

void Map::generate_points(int num, int width, int height, int len) {
    for (size_t i = 0; i < num; i++)
        points.push_back(gen_Point(width, height, len, 0));
}

Coordinates Map::gen_Point(int width, int height, int len, int min_x, int min_y) {

    int x_r = rand() % (width - min_x) + min_x;
    int y_r = rand() % (height - min_y) + min_y;
    int phi = rand() % 330;
    //std::cout << x_r << " " << y_r << "\n";
    Coordinates test_point(x_r, y_r, phi, len);//, phi);

    if (!is_valid(test_point)) {
        return test_point;
    } else {
        return gen_Point(width, height, len, min_x, min_y);
    }

}
/*

Obstacle Map::gen_rand_obstacle(int width, int height, double min_diag, double max_diag) {
    Coordinates new_min = gen_Point(width, height);
    Coordinates new_max = gen_Point(width, height, new_min.x, new_min.y);

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
*/
/*

void Map::save(const char * file_for_obstacles, const char * file_for_points) {
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
*/

bool Map::is_point_in_obstacle(Coordinates point)
{
    for (size_t i = 0; i < obstacles.size(); i++) {
        if (obstacles[i].min_vec.x < point.x &&
            obstacles[i].max_vec.x > point.x &&
            obstacles[i].min_vec.y < point.y &&
            obstacles[i].max_vec.y > point.y) {
            return true;
        }
    }
    return false;
}
/*

bool Map::check_line(Coordinates point_1, Coordinates point_2) {
    bool x_y;
    if (point_1.x == point_2.x) { x_y = 1;}
    if (point_1.y == point_2.y) { x_y = 0;}

    int const_coord = (x_y) ? point_1.x : point_1.y;
    int mov_coord = (x_y) ? std::min(point_1.y, point_2.y) :
                    std::min(point_1.x, point_2.x);
    int coord_fin = (x_y) ? std::max(point_1.y, point_2.y) :
                    std::max(point_1.x, point_2.x);
    //std::cout << "*/
/* message *//*
/////////////////////////////////" << std::endl;
    for (size_t i = 0; mov_coord != coord_fin; i++) {
        Coordinates temp_point = (x_y) ? Coordinates(const_coord, mov_coord) :
                          Coordinates(mov_coord, const_coord);
        if (!is_point_in_obstacle(temp_point)) {
            return false;
        } else {
            mov_coord++;
        }
    }

    return true;

}
*/
/*

bool Map::obstacle_intersection(Obstacle new_one) {

    if (check_line(new_one.min_vec, Coordinates(new_one.min_vec.x, new_one.max_vec.y)) &&
        check_line(new_one.min_vec, Coordinates(new_one.max_vec.x, new_one.min_vec.y)) &&
        check_line(Coordinates(new_one.min_vec.x, new_one.max_vec.y), new_one.max_vec) &&
        check_line(Coordinates(new_one.max_vec.x, new_one.min_vec.y), new_one.max_vec)){
        return true;
    }

    return false;
}

*/


bool Map::among_points(Coordinates point) {
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].coords == point.coords) { return true; }
    }

    return false;
}

bool Map::is_valid(Coordinates object) {

    int end_x = object.x + object.length * cos(object.phi * M_PI / 180);
    int end_y = object.y + object.length * sin(object.phi * M_PI / 180);

    if (end_x >= width || end_x <= 0 || end_y <= 0 || end_y >= height) {
        return true;
    }

    Coordinates end(end_x, end_y);
    double * equation = new double[3];
    Geometry::get_equation(equation, object, end);

    for (int i = std::min(end_y, (int)object.y); i < std::max(end_y, (int)object.y); i++) {
        double line[3] = {0, 1, (double) (-1 * (i))};
        double x;
        if (Geometry::get_intersection(line, equation, &x)) {
            Coordinates pt(x, i); // запиливаем
            if (is_point_in_obstacle(pt)) {
                return true;
            }
        }
    }

    return false;
}
