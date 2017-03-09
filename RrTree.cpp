#include <cmath>
#include <iostream>
#include "RrTree.h"
#include "Render.h"

RrTree::RrTree(Map* the_map, double distance) :
        goal_state(RrtNode(the_map->points.back())),
        min_distance(distance)
{
    nodes.push_back(RrtNode(the_map->points.front()));
}

void RrTree::search(Map* the_map, bool search, Bitmap * bmp) {
    //double temp[2] = {the_map->points.front().x, the_map->points.front().y};
    KdTree kd;
    kd.nodes.push_back(KdNode(&the_map->points.front().coords, 0));
    //std::cout << nodes.size() << std::endl;
    while (is_available_second(the_map, nodes.back().point, goal_state.point)) {


        the_map->generate_points(1, the_map->width, the_map->height, the_map->points[0].length);
        //std::cout << nodes.size() << std::endl;
        /*std::cout << nodes.back().point.x << " "
                  << nodes.back().point.y << " "
                  << nodes.back().point.phi << "\n";
        */this->extend(the_map, &kd, search, bmp);
        //std::cout //<< nodes.back().point.x << " "
                  //<< nodes.back().point.y << " ";
                  //<< nodes.back().point.phi << "\n";
        ///std::cout << nodes.size() << std::endl;
    }/*
    get_path(nodes.size() - 1);
    render_path(path, bmp, 1);
    bmp->out_bmp("MAP_PATH.bmp");*/
}

void RrTree::extend(Map* the_map, KdTree * kd, bool search, Bitmap * bmp) {

    static int counter = 0;
    static double current = 0;

    Coordinates new_point = the_map->points.back(); // взяли только что сгенерир точку
    double best_distance = the_map->width * the_map->height;
    int best_index = -1;
    if (search) {
        //std::cout << "Sdsdsdsd" << std::endl;
        kd->seek_nearest_with_kd(0, new_point.coords, 0, best_index, best_distance);
    } else {
        for (size_t i = 0; i < nodes.size(); i++) {
            double tmp = get_distance(new_point, nodes[i].point);
            if (tmp < best_distance) {
                best_index = i;
                best_distance = tmp;
            }
        }
    }
    //std::cout << "Sdsdsdsd" << std::endl;
    if (!is_available_second(the_map, new_point, nodes[best_index].point)  &&
        sqrt(best_distance) <= this->min_distance) {

        nodes.push_back(RrtNode(new_point, best_index));
        kd->push(new_point.coords, 0, -1, -1);
        nodes[best_index].children.push_back(nodes.size() - 1);

        if (bmp) {
            //out in .bmp
            for (size_t i = 0; i < nodes.size(); i++) {
                go(i);
                if (edges.size())
                    render_path(edges, bmp, 0);
            }
            bmp->out_bmp("MAP_PATH.bmp");

        }
        //info to cmd
        counter++;
        if (counter % 50) {
            if (this->nodes.back().point.x > current) {
                current = this->nodes.back().point.x;
                //std::cout << current / 10.0 << std::endl;
            }
        }


    } else {
        the_map->points.pop_back();
        return;
    }
}

void RrTree::get_path(int index) {
    if (index == nodes.size() - 1) {
        path.push_back(goal_state.point);
    }
    path.push_back(nodes[index].point);
    if (nodes[index].parent != -1) {
        get_path(nodes[index].parent);
    }
}

double RrTree::get_distance(Coordinates point_1, Coordinates point_2) {
    return (point_1.x - point_2.x)*(point_1.x - point_2.x) +
           (point_1.y - point_2.y)*(point_1.y - point_2.y);

}

bool RrTree::is_available(Map* the_map, Coordinates point_1, Coordinates point_2) {

    //std::cout << "Sdsdsdsd" << std::endl;
    bool ** temp_plain = new bool * [width];
    for (size_t i = 0; i < width; i++) {
        temp_plain[i] = new bool [height];
    }
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            temp_plain[i][j] = false;
        }
    }

    bresenham_obj(temp_plain, point_1, point_2);
    //std::cout << "Sdsdsdsd" << std::endl;
    //фигура закрашена


    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (temp_plain[i][j]) { // если точка единичная, значит она в маршруте
                // если она в маршруте, значит она не должна быть в препятствии
                // проверим
                //сделаем из нее точку
                Coordinates pt((double)i, (double)j);
                //std::cout << "Sdsdsdsd" << std::endl;
                if (the_map->is_point_in_obstacle(pt)) { // если эта точка в обстакле, то
                    for (size_t k = 0; k < width; k++) {
                        delete [] temp_plain[k];
                    }
                    //std::cout << "YOU SHELL NOT PAAAAASS!!!" << std::endl;
                    return true;
                }
            }
        }
    }

    for (size_t k = 0; k < width; k++) {
        delete [] temp_plain[k];
    }

    return false;
}

void RrTree::go(int index) {
    edges.clear();
    
    Coordinates temp = nodes[index].point;

    if (nodes[index].children.size() != 0) {
        for (size_t i = 0; i < nodes[index].children.size(); i++) {
            this->edges.push_back(temp);
            this->edges.push_back(nodes[nodes[index].children[i]].point);
        }
    }
}

void RrTree::optimize_path(Map * map, int step, int iter) {

    while (iter) {
        for (int i = 0; i < path.size() - step; i++) {
            if (!is_available_second(map, path[i], path[i + step])) {
                path.erase(path.begin() + i + 1, path.begin() + i + step - 1);
            }
        }
        iter--;
    }
}

static
void get_equation(double * coefs, Coordinates one, Coordinates two) {
    coefs[0] = two.y - one.y;
    if (coefs[0] == 0) {
        coefs[1] = 1;
        coefs[2] = -1*two.y;
        return;
    }
    coefs[1] = two.x - one.x;
    if (coefs[1] == 0) {
        coefs[0] = 1;
        coefs[2] = -1*two.x;
        return;
    }
    coefs[2] = (-1 * one.x) * coefs[0] + one.y * coefs[1];
    coefs[1] *= -1;
}

static
bool get_intersection(double * firstLineCoefs, double * secondLineCoefs, double * x) {

    double det = firstLineCoefs[0] * secondLineCoefs[1] - firstLineCoefs[1] * secondLineCoefs[0];
    double det_1 = -1*firstLineCoefs[2] * secondLineCoefs[1] - firstLineCoefs[1] * -1*secondLineCoefs[2];
    double det_2 = firstLineCoefs[0] * -1*secondLineCoefs[2] - -1*firstLineCoefs[2] * secondLineCoefs[0];

    if (det) {
        *x = det_1/det;
        return true;
    } else {

        if (!det_1 && !det_2) {
            *x = -1*secondLineCoefs[2];
            return true;
        } else {
            return false;
        }

    }
}

static
void up_right_left(int &first, int &second, int intersections, int i, int end_left_y, int end_right_y, bool right) {
    if (intersections == 3) {
        if (i < std::min(end_left_y, end_right_y)) {
            first   = (right) ? 0 : 1;
            second  = (right) ? 1 : 2;
        } else {
            first   = (right) ? 1 : 0;
            second  = (right) ? 2 : 1;
        }
    } else {
        first   = (intersections == 2) ? 0 : 1;
        second  = (intersections == 2) ? 1 : 2;
    }
}

bool RrTree::is_available_second(Map *the_map, Coordinates object_1, Coordinates object_2) {

    std::vector <std::pair<Coordinates, Coordinates>> ribs;
    std::vector <double*> equations;

    Coordinates left = (object_1.x > object_2.x) ? object_2 : object_1;
    Coordinates right = (object_1.x < object_2.x) ? object_2 : object_1;

    Coordinates up = (object_1.y < object_2.y) ? object_2 : object_1;
    Coordinates down = (object_1.y > object_2.y) ? object_2 : object_1;

    int end_left_x = left.x + left.length * cos(object_1.phi * M_PI / 180);
    int end_left_y = left.y + left.length * sin(object_1.phi * M_PI / 180);
    Coordinates end_left(end_left_x, end_left_y);

    int end_right_x = right.x + right.length * cos(object_1.phi * M_PI / 180);
    int end_right_y = right.y + right.length * sin(object_1.phi * M_PI / 180);
    Coordinates end_right(end_right_x, end_right_y);

    int end_up_x = up.x + up.length * cos(object_1.phi * M_PI / 180);
    int end_up_y = up.y + up.length * sin(object_1.phi * M_PI / 180);
    Coordinates end_up(end_up_x, end_up_y);

    int end_down_x = down.x + down.length * cos(object_1.phi * M_PI / 180);
    int end_down_y = down.y + down.length * sin(object_1.phi * M_PI / 180);
    Coordinates end_down(end_down_x, end_down_y);


    if (up == right && up.y <= end_left_y) {
        std::cout << "one\n";
        ribs.push_back(std::make_pair(left, end_left));
        if (left.y != right.y) ribs.push_back(std::make_pair(left, right));
        ribs.push_back(std::make_pair(right, end_right));
        if (left.y != right.y) ribs.push_back(std::make_pair(end_left, end_right));

    } else if (up == left && up.y <= end_right_y ||
               up == right && up.y >= end_left_y ||
               up == left && up.y >= end_right_y) {
        std::cout << "two\n";
        ribs.push_back(std::make_pair(left, end_left));
        if (left.y != right.y) ribs.push_back(std::make_pair(left, right));
        if (left.y != right.y) ribs.push_back(std::make_pair(end_left, end_right));
        ribs.push_back(std::make_pair(right, end_right));

    } else if (up.x == down.x && (object_1.phi != 90 && object_1.phi != 270)) {
        std::cout << "herrrrrrrrrrrrrrrrrre\n";
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
        get_equation(equations[i], ribs[i].first, ribs[i].second);
    }

    int lower_bound_y = std::min(down.y, down.y + down.length * sin(object_1.phi * M_PI / 180));
    int higher_bound_y = std::max(up.y, up.y + up.length * sin(object_1.phi * M_PI / 180));
    int lower_bound_x = std::min((int)left.x, end_left_x);
    int higher_bound_x = std::max((int)right.x, end_left_x + (int)right.x - (int)left.x);

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
            if (get_intersection(line, item, &x)) {
                if (x >= lower_bound_x && x <= higher_bound_x)
                    x_intersect.push_back(x);
            }
        }

        if (x_intersect.size() >= 2) {
            std::cout << x_intersect.size() << std::endl;

            int first_index;
            int second_index;

            if (up == right && i <= up.y && i >= end_left_y && object_1.phi > 0 && object_1.phi < 180 ||
                up == left && i >= end_right_y && i <= left.y && object_1.phi > 0 && object_1.phi < 180) {
                std::cout << "here\n";
                std::cout << x_intersect.size() << std::endl;
                first_index = 1;
                second_index = 2;

            } else {
                if (up == right)
                    up_right_left(first_index, second_index, x_intersect.size(), i, end_left_y, end_right_y, 1);
                else if (up == left) {
                    up_right_left(first_index, second_index, x_intersect.size(), i, end_left_y, end_right_y, 0);
                } else if (right == left && (object_1.phi < 90 || object_1.phi > 270)) {
                    up_right_left(first_index, second_index, x_intersect.size(), i, end_down_y, (int)up.y, 1);
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
                    up_right_left(first_index, second_index, x_intersect.size(), i, (int)up.y, end_down_y, 0);
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


