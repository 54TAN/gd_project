#include "Map.h"
#include "Render.h"

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
    if (!obstacles.size()){
        for (size_t i = 0; i < num; i++) {
            points.push_back(gen_Point(width, height, 0, 0));
        }
    } else {
        while (num) {
            Coordinates new_point = gen_Point(width, height, len);
            if (is_point_in_obstacle(new_point)) {
                points.push_back(new_point);
                num--;
            }
        }
    }
}

Coordinates Map::gen_Point(int width, int height, int len, int min_x, int min_y) {

    int x_r = rand() % (width - min_x) + min_x;
    int y_r = rand() % (height - min_y) + min_y;
    int phi = rand() % 360;

    Coordinates test_point(x_r, y_r, phi);

    if (!is_valid(test_point)) {
        return test_point;
    } else {
        return gen_Point(width, height, min_x, min_y, len);
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
    if (obstacles.size() == 0) {
        return false;
    }

    bool ** temp_plain = new bool * [width];
    for (size_t i = 0; i < width; i++) {
        temp_plain[i] = new bool [height];
    }
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            temp_plain[i][j] = false;
        }
    }
    double end_x = object.x + object.length * cos(object.phi * M_PI / 180);
    double end_y = object.y + object.length * sin(object.phi * M_PI / 180);
    Coordinates end(end_x, end_y);
    bresenham(temp_plain, object, end, 0);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {

            if (temp_plain[i][j]) { // если что-то есть


                Coordinates pt((double)i, (double)j); // запиливаем
                if (!is_point_in_obstacle(pt)) { // каждую точку
                    for (size_t k = 0; k < width; k++) {
                        delete [] temp_plain[k];
                    }
                    delete temp_plain;
                    //std::cout << "YOU SHELL NOT PAAAAASS!!!" << std::endl;
                    return true;
                }


            }

        }
    }

    for (size_t k = 0; k < width; k++) {
        delete [] temp_plain[k];
    }
    delete temp_plain;

    return false;
}
