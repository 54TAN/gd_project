#include "Check.h"
#include "Render.h"
#include "Consts.h"
#include "Geometry.h"

#include <iostream>
#include <vector>
#include <cmath>

bool Check::check_brick(Map *the_map, Coordinates object_1, Coordinates object_2)
{
	std::vector <std::pair<Coordinates, Coordinates>> ribs;
    std::vector <int*> equations;
//уже есть четыре координаты концов
//добавляем верхний, нижниий, левый, правый
	Coordinates left = (object_1.x > object_2.x) ? object_2 : object_1;
    Coordinates right = (object_1.x < object_2.x) ? object_2 : object_1;

    Coordinates up = (object_1.y < object_2.y) ? object_2 : object_1;
    Coordinates down = (object_1.y > object_2.y) ? object_2 : object_1;

    int end_left_x = left.x + left.length * cos(object_1.phi * M_PI / 180);
    if (end_left_x < 0 || end_left_x >= WIDTH) return true;
    int end_left_y = left.y + left.length * sin(object_1.phi * M_PI / 180);
    if (end_left_y < 0 || end_left_y >= HEIGTH) return true;
    Coordinates end_left(end_left_x, end_left_y);

    int end_right_x = right.x + right.length * cos(object_1.phi * M_PI / 180);
    if (end_right_x < 0 || end_right_x >= WIDTH) return true;
    int end_right_y = right.y + right.length * sin(object_1.phi * M_PI / 180);
    if (end_right_y < 0 || end_right_y >= HEIGTH) return true;
    Coordinates end_right(end_right_x, end_right_y);

    int end_up_x = up.x + up.length * cos(object_1.phi * M_PI / 180);
    if (end_up_x < 0 || end_up_x >= WIDTH) return true;
    int end_up_y = up.y + up.length * sin(object_1.phi * M_PI / 180);
    if (end_up_y < 0 || end_up_y >= HEIGTH) return true;
    Coordinates end_up(end_up_x, end_up_y);

    int end_down_x = down.x + down.length * cos(object_1.phi * M_PI / 180);
    if (end_down_x < 0 || end_down_x >= WIDTH) return true;
    int end_down_y = down.y + down.length * sin(object_1.phi * M_PI / 180);
    if (end_down_y < 0 || end_down_y >= HEIGTH) return true;
    Coordinates end_down(end_down_x, end_down_y);


    if (up == right && up.y <= end_left_y) {
        //std::cout << "one\n";
        ribs.push_back(std::make_pair(left, end_left));
        ribs.push_back(std::make_pair(left, right));
        ribs.push_back(std::make_pair(right, end_right));
        ribs.push_back(std::make_pair(end_left, end_right));

    } else if (up == left && up.y <= end_right_y ||
               up == right && up.y >= end_left_y ||
               up == left && up.y >= end_right_y) {
        //std::cout << "two\n";
        ribs.push_back(std::make_pair(left, end_left));
        ribs.push_back(std::make_pair(left, right));
        ribs.push_back(std::make_pair(end_left, end_right));
        ribs.push_back(std::make_pair(right, end_right));

    } else if (up.x == down.x && (object_1.phi != 90 && object_1.phi != 270)) {
        //std::cout << "herrrrrrrrrrrrrrrrrre\n";
        if (object_1.phi < 90 || object_1.phi > 270) {

            ribs.push_back(std::make_pair(up, down));
            ribs.push_back(std::make_pair(down, end_down));
            ribs.push_back(std::make_pair(end_down, end_up));
            ribs.push_back(std::make_pair(up, end_up));
        } else {

            ribs.push_back(std::make_pair(end_down, end_up));
            ribs.push_back(std::make_pair(down, end_down));
            ribs.push_back(std::make_pair(up, down));
            ribs.push_back(std::make_pair(up, end_up));
        }

    } else if (up.x == down.x && (object_1.phi == 90 || object_1.phi == 270)) {

        ribs.push_back(std::make_pair(up, down));

    } else {
        std::cout << "no!\n";
        return 0;
    }

    for (size_t i = 0; i < 4; i++) {
        equations.push_back(new int[3]);
        Geometry::get_equation(equations[i], ribs[i].first, ribs[i].second);
    }
/*
	std::cout << "equations : \n";
	for (size_t i = 0; i < 4; i++) {
		std::cout << equations[i][0] << " " << equations[i][1] << " " << equations[i][2] << "\n";
	}
*/
	//чекаем кажду контрольную точку
	for (size_t i = 0; i < the_map->control_points.size(); i++) {
		Coordinates current = the_map->control_points[i];
		if (current.x * equations[0][0] + current.y * equations[0][1] <=  -1*equations[0][2] &&
			current.x * equations[1][0] + current.y * equations[1][1] >= -1*equations[1][2] &&
			current.x * equations[2][0] + current.y * equations[2][1] <= -1*equations[2][2] &&
			current.x * equations[3][0] + current.y * equations[3][1] >= -1*equations[3][2] )
			return true;
	}

	return false;
}

bool Check::check_slice(Map *the_map, Coordinates object_1, Coordinates object_2)
{
	std::vector <std::pair<Coordinates, Coordinates>> pizza(3, std::make_pair(object_1, object_2));
    std::vector <int*> pizza_equations(2);

    Coordinates more = (object_1.phi < object_2.phi) ? object_2 : object_1;
    Coordinates less = (object_1.phi > object_2.phi) ? object_2 : object_1;

    int end_less_x = object_2.x + object_2.length * cos(less.phi * M_PI / 180);
    if (end_less_x < 0 || end_less_x >= WIDTH) return true;
    int end_less_y = object_2.y + object_2.length * sin(less.phi * M_PI / 180);
    if (end_less_y < 0 || end_less_y >= WIDTH) return true;
    Coordinates end_less(end_less_x, end_less_y);

    int end_more_x = object_2.x + object_2.length * cos(more.phi * M_PI / 180);
    if (end_more_x < 0 || end_more_x >= WIDTH) return true;
    int end_more_y = object_2.y + object_2.length * sin(more.phi * M_PI / 180);
    if (end_more_x < 0 || end_more_x >= WIDTH) return true;
    Coordinates end_more(end_more_x, end_more_y);

    size_t circle_index = (more.phi < 180) ? 2 : 1;
    size_t less_index = (more.phi < 180) ? 1 : 0;
    size_t more_index = (more.phi < 180) ? 0 : 2;

    int low_bound_x = std::min((int)object_2.x, std::min(end_less_x, end_more_x));
    int low_bound_y = std::min((int)object_2.y, std::min(end_less_y, end_more_y));
    int high_bound_x = std::max((int)object_2.x, std::max(end_less_x, end_more_x));
    int high_bound_y = std::max((int)object_2.y,std::max(end_less_y, end_more_y));


    if (less.phi < 90 && more.phi > 90) {
        high_bound_y = object_2.y + object_2.length;
        more_index = 0;
        less_index = 1;
        circle_index = 2;
    }

    if (more.phi > 270 && less.phi < 270 && abs(less.phi - more.phi) < 180) {
        low_bound_y = object_2.y - object_2.length;
        less_index = 1;
        circle_index = 0;
        more_index = 2;
    }

    if (less.phi < 90 && more.phi > less.phi + 180) {
        //std::cout << "been there\n";
        high_bound_x = object_2.x + object_2.length;
        more_index = 0;   //1
        circle_index = 2;  //0
        less_index = 1;   //2
        if (more.phi > less.phi + 180) {
            high_bound_y = end_less_y;
            //low_bound_y = object_2.y - object_1.length;
            if (more.phi > 270) low_bound_y = end_more_y;
        }
    }

    if (less.phi < 180 && more.phi > 180) {
        //std::cout << "been there\n";
        if (abs(less.phi - more.phi) < 180) {
            circle_index = 0; // 0
            more_index = 1;  // 1
            less_index = 2;   // 2
            low_bound_x = object_2.x - object_2.length;
        }
        if (more.phi > less.phi + 180) {
            circle_index = 2; // 0
            more_index = 0;  // 1
            less_index = 1;   // 2
            high_bound_y = end_less_y;
            low_bound_y = end_more_y;
        }
    }

    if (abs(end_less.x - end_more.x) <= 1 && abs(less.phi - more.phi) > 179) {
        low_bound_x = object_2.x;
        high_bound_x = object_2.x + object_2.length;
    }

    if (low_bound_x < 0 || low_bound_x >= WIDTH) return true;
    if (high_bound_x < 0 || high_bound_x >= WIDTH) return true;
    if (low_bound_y < 0 || low_bound_y >= HEIGTH) return true;
    if (high_bound_y < 0 || high_bound_y >= HEIGTH) return true;


    pizza[circle_index] = std::make_pair(less, less);
    pizza[less_index] = std::make_pair(object_2, end_less);
    pizza[more_index] = std::make_pair(object_2, end_more);

	Geometry::get_equation(pizza_equations[0], pizza[less_index].first, pizza[less_index].second);
	Geometry::get_equation(pizza_equations[1], pizza[more_index].first, pizza[more_index].second);
/*
	for (size_t i = 0; i < the_map->control_points.size(); i++) {
		Coordinates current = the_map->control_points[i];
		if (pow((current.x - less.x), 2) + pow((current.y - less.y), 2) <= object_1.length &&
			current.x * pizza_equations[1][0] + current.y * pizza_equations[1][1] >= -1*pizza_equations[1][2] &&
			current.x * pizza_equations[2][0] + current.y * pizza_equations[2][1] <= -1*pizza_equations[2][2] )
			return true;
	}
*/
    return false;
	
}


