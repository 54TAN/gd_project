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
    std::vector <double*> equations;

    Coordinates left = (object_1.x > object_2.x) ? object_2 : object_1;
    Coordinates right = (object_1.x < object_2.x) ? object_2 : object_1;

    Coordinates up = (object_1.y < object_2.y) ? object_2 : object_1;
    Coordinates down = (object_1.y > object_2.y) ? object_2 : object_1;

    int end_left_x = left.x + left.length * cos(object_1.phi * M_PI / 180);
    if (end_left_x < 0 || end_left_x >= width) return true;
    int end_left_y = left.y + left.length * sin(object_1.phi * M_PI / 180);
    if (end_left_y < 0 || end_left_y >= height) return true;
    Coordinates end_left(end_left_x, end_left_y);

    int end_right_x = right.x + right.length * cos(object_1.phi * M_PI / 180);
    if (end_right_x < 0 || end_right_x >= width) return true;
    int end_right_y = right.y + right.length * sin(object_1.phi * M_PI / 180);
    if (end_right_y < 0 || end_right_y >= height) return true;
    Coordinates end_right(end_right_x, end_right_y);

    int end_up_x = up.x + up.length * cos(object_1.phi * M_PI / 180);
    if (end_up_x < 0 || end_up_x >= width) return true;
    int end_up_y = up.y + up.length * sin(object_1.phi * M_PI / 180);
    if (end_up_y < 0 || end_up_y >= height) return true;
    Coordinates end_up(end_up_x, end_up_y);

    int end_down_x = down.x + down.length * cos(object_1.phi * M_PI / 180);
    if (end_down_x < 0 || end_down_x >= width) return true;
    int end_down_y = down.y + down.length * sin(object_1.phi * M_PI / 180);
    if (end_down_y < 0 || end_down_y >= height) return true;
    Coordinates end_down(end_down_x, end_down_y);


    if (up == right && up.y <= end_left_y) {
        //std::cout << "one\n";
        ribs.push_back(std::make_pair(left, end_left));
        if (left.y != right.y) ribs.push_back(std::make_pair(left, right));
        ribs.push_back(std::make_pair(right, end_right));
        if (left.y != right.y) ribs.push_back(std::make_pair(end_left, end_right));

    } else if (up == left && up.y <= end_right_y ||
               up == right && up.y >= end_left_y ||
               up == left && up.y >= end_right_y) {
        //std::cout << "two\n";
        ribs.push_back(std::make_pair(left, end_left));
        if (left.y != right.y) ribs.push_back(std::make_pair(left, right));
        if (left.y != right.y) ribs.push_back(std::make_pair(end_left, end_right));
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


    for (int i = 0; i < ribs.size(); i++) {
        equations.push_back(new double [3]);
        Geometry::get_equation(equations[i], ribs[i].first, ribs[i].second);
    }

    int lower_bound_y = std::min(down.y, down.y + down.length * sin(object_1.phi * M_PI / 180));
    int higher_bound_y = std::max(up.y, up.y + up.length * sin(object_1.phi * M_PI / 180));
    int lower_bound_x = std::min((int)left.x, end_left_x);
    int higher_bound_x = std::max((int)right.x, end_left_x + (int)right.x - (int)left.x);

    if (lower_bound_x < 0 || lower_bound_x >= width) return true;
    if (higher_bound_x < 0 || higher_bound_x >= width) return true;
    if (lower_bound_y < 0 || lower_bound_y >= height) return true;
    if (higher_bound_y < 0 || higher_bound_y >= height) return true;

    /*std::cout << "bounds : " << lower_bound_y << " " << higher_bound_y << "\n";
    std::cout << "bounds : " << lower_bound_x << " " << higher_bound_x << "\n";*/

    if (lower_bound_y == higher_bound_y) {
        for (int j = lower_bound_x; j < higher_bound_x; j++) {
            Coordinates pt((double)j, (double)lower_bound_y);
            if (the_map->is_point_in_obstacle(pt)) {
                return true;
            }
        }
    }


    for (int i = lower_bound_y; i < higher_bound_y; i++) { //по высоте

        double line[3] = {0, 1, (double) (-1 * (i))};

        double x = -1;
        std::vector<double> x_intersect;

        for (auto item : equations) {
            if (Geometry::get_intersection(line, item, &x)) {
                if (x >= lower_bound_x && x <= higher_bound_x)
                    x_intersect.push_back(x);
            }
        }

        if (x_intersect.size() >= 2) {
            //std::cout << x_intersect.size() << std::endl;

            int first_index;
            int second_index;

            if (up == right && i <= up.y && i >= end_left_y && object_1.phi > 0 && object_1.phi < 180 ||
                up == left && i >= end_right_y && i <= left.y && object_1.phi > 0 && object_1.phi < 180) {
                //std::cout << "here\n";
                //std::cout << x_intersect.size() << std::endl;
                first_index = 1;
                second_index = 2;

            } else {
                if (up == right)
                    Geometry::up_right_left(first_index, second_index, x_intersect.size(), i, end_left_y, end_right_y, 1);
                else if (up == left) {
                    Geometry::up_right_left(first_index, second_index, x_intersect.size(), i, end_left_y, end_right_y, 0);
                } else if (right == left && (object_1.phi < 90 || object_1.phi > 270)) {
                    Geometry::up_right_left(first_index, second_index, x_intersect.size(), i, end_down_y, (int)up.y, 1);
                    /*if (x_intersect.size() == 3) {
                        if (i <= std::min(end_down_y, (int)up.y)) {
                            first_index   = 0;
                            second_index  = 1;
                        } else {
                            first_index   = 1;
                            second_index  = 2;
                        }
                    } else {
                        first_index   = (x_intersect.size() == 2) ? 0 : 1;
                        second_index  = (x_intersect.size() == 2) ? 1 : 2;
                    }*/
                } else if (right == left && (object_1.phi > 90 && object_1.phi < 270)) {
                    //std::cout << "herreSSSSSSSSSSSSSSSSSSSSS\n";
                    Geometry::up_right_left(first_index, second_index, x_intersect.size(), i, (int)up.y, end_down_y, 0);
                    /*if (x_intersect.size() == 3) {
                        if (i <= std::min(end_down_y, (int)up.y)) {
                            first_index   = 1;
                            second_index  = 2;
                        } else {
                            first_index   = 0;
                            second_index  = 1;
                        }
                    } else {
                        first_index   = (x_intersect.size() == 2) ? 0 : 1;
                        second_index  = (x_intersect.size() == 2) ? 1 : 2;
                    }*/
                } else {
                    std::cout << "there is no options for this case\n";
                    return false;
                }
            }

            //std::cout << first_index << " : " << second_index << "\n";
            for (int j = lower_bound_x; j < higher_bound_x; j++) {
                if ((x_intersect[first_index] <= j && j <= x_intersect[second_index]) ||
                    (x_intersect[first_index] >= j && j >= x_intersect[second_index])) {
                    Coordinates pt((double)j, (double)i);
                    if (the_map->is_point_in_obstacle(pt)) {
                        return true;
                    }
                }
            }
        }  else if (x_intersect.size() == 1 && (object_1.phi == 90 || object_1.phi == 270)) {
            int j = x_intersect[0];
            Coordinates pt((double)j, (double)i);
            if (the_map->is_point_in_obstacle(pt)) {
                return true;
            }
        }
    }
	
	return false;
}

bool Check::check_slice(Map *the_map, Coordinates object_1, Coordinates object_2)
{
	std::vector <std::pair<Coordinates, Coordinates>> pizza(3, std::make_pair(object_1, object_2));
    std::vector <double*> pizza_equations;

    Coordinates more = (object_1.phi < object_2.phi) ? object_2 : object_1;
    Coordinates less = (object_1.phi > object_2.phi) ? object_2 : object_1;

    int end_less_x = object_2.x + object_2.length * cos(less.phi * M_PI / 180);
    if (end_less_x < 0 || end_less_x >= width) return true;
    int end_less_y = object_2.y + object_2.length * sin(less.phi * M_PI / 180);
    if (end_less_y < 0 || end_less_y >= width) return true;
    Coordinates end_less(end_less_x, end_less_y);

    int end_more_x = object_2.x + object_2.length * cos(more.phi * M_PI / 180);
    if (end_more_x < 0 || end_more_x >= width) return true;
    int end_more_y = object_2.y + object_2.length * sin(more.phi * M_PI / 180);
    if (end_more_x < 0 || end_more_x >= width) return true;
    Coordinates end_more(end_more_x, end_more_y);

    size_t circle_index = (more.phi < 180) ? 2 : 1;
    size_t less_index = (more.phi < 180) ? 1 : 0;
    size_t more_index = (more.phi < 180) ? 0 : 2;

//    тривиальные границы, в ветвлениях, они скоррекстируются
//    тривиальные - это без учета окружнстей, только по вершинам
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

    if (low_bound_x < 0 || low_bound_x >= width) return true;
    if (high_bound_x < 0 || high_bound_x >= width) return true;
    if (low_bound_y < 0 || low_bound_y >= height) return true;
    if (high_bound_y < 0 || high_bound_y >= height) return true;


    pizza[circle_index] = std::make_pair(less, less);
    pizza[less_index] = std::make_pair(object_2, end_less);
    pizza[more_index] = std::make_pair(object_2, end_more);

    for (int i = 0; i < pizza.size(); i++) {
        if (i != circle_index){
            pizza_equations.push_back(new double [3]);
            Geometry::get_equation(pizza_equations[i], pizza[i].first, pizza[i].second);
        } else {
            pizza_equations.push_back(NULL);
        }
    }

    for (int i = low_bound_y; i <= high_bound_y; i++) { //по высоте

        double line[3] = {0, 1, (double) (-1 * (i))};

        double x = -1;
        std::vector<double> x_intersect;

        bool only_circle = false;

        for (int i = 0; i < pizza_equations.size(); i++) {

            if (i != circle_index) {
                if (Geometry::get_intersection(line, pizza_equations[i], &x)) {
                    if (x >= low_bound_x && x <= high_bound_x) {
                        x_intersect.push_back(x);
                        //std::cout << "here ";
                    }
                }
            } else {
                std::pair <Coordinates*, Coordinates*> pair =
                        std::make_pair(new Coordinates(-1, -1), new Coordinates(-1, -1));

                if (Geometry::get_intersection_circle(line, object_2, pair)) {
                    if (pair.first->x == pair.second->x) {
                        //one intersecrion
                        if (pair.first->x <= high_bound_x && pair.first->x >= low_bound_x) {
                            x_intersect.push_back(pair.first->x);
                            only_circle = true;
                        }
                        //x_intersect.push_back(pair.first->x);
                        //only_circle = true;
                    } else {
                        //x_intersect.clear();


                        if (pair.first->x <= high_bound_x && pair.first->x >= low_bound_x) {
                            x_intersect.push_back(pair.first->x);
                            //std::cout << "crcl\n";
                        }
                        if (pair.second->x <= high_bound_x && pair.second->x >= low_bound_x) {
                            x_intersect.push_back(pair.second->x);
                            //std::cout << "crcl\n";
                        }
                        if (pair.first->x <= high_bound_x && pair.first->x >= low_bound_x &&
                            pair.second->x <= high_bound_x && pair.second->x >= low_bound_x) {
                            //std::cout << "TWO\n";
                            x_intersect.clear();
                            x_intersect.push_back(pair.first->x);
                            x_intersect.push_back(pair.second->x);

                        }
                        //break;
                    }
                }
            }
        }


        int first_index = (x_intersect.size() == 2) ? 0 : 1;
        int second_index = (x_intersect.size() == 2) ? 1 : 2;

        //std::cout << x_intersect.size() << " i : " << i <<"\n";
        if (x_intersect.size() == 3 && i < object_2.y && less.phi > 45) {
            first_index = 0;
            second_index = 1;
            //std::cout << "f : " << first_index << " s : " << second_index << "\n";
            //std::cout << x_intersect[0] << " : " << x_intersect[1] << " : " << x_intersect[2] << "\n";
        }



        if (x_intersect.size() > 1) {
            //std::cout << "in here\n";
            for (int j = low_bound_x; j <= high_bound_x; j++) {
                if ((x_intersect[first_index] <= j && j <= x_intersect[second_index]) ||
                    (x_intersect[first_index] >= j && j >= x_intersect[second_index])) {
                    //std::cout << "HERRRRE\n";
                    Coordinates pt((double)j, (double)i);
                    if (the_map->is_point_in_obstacle(pt)) {
                        return true;
                    }

                }
            }
        } else if (x_intersect.size() == 1 && only_circle) {
            //std::cout << "here\n";
            Coordinates pt((double)(int)x_intersect[0], (double)i);
            if (the_map->is_point_in_obstacle(pt)) {
                return true;
            }
        }

    }


    return false;
	
}

bool Check::check_line(Map* map, Coordinates object_1, Coordinates object_2) 
{
    bool ** some;
    std::vector<Coordinates> coords_for_check;
    bresenham(some, object_1, object_2, 0, 0, 0, 0, &coords_for_check);
    for (auto item : coords_for_check) {
        if (map->is_point_in_obstacle(item)) {
            return true;
        }
    }
    return false;
}
