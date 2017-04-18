#include <cmath>
#include <cassert>
#include <iostream>
#include <string>

#include "RrTree.h"
#include "Render.h"
#include "Geometry.h"
#include "Check.h"

#define _USE_MATH_DEFINES

RrTree::RrTree(Map* the_map, double distance) :
        goal_state(RrtNode(the_map->points.back())),
        min_distance(distance)
{
    nodes.push_back(RrtNode(the_map->points.front()));
}

void RrTree::search(Map* the_map, bool search, Bitmap * bmp) 
{
    KdTree kd;
    kd.nodes.push_back(KdNode(&the_map->points.front().coords, 0));
    while (is_available(the_map, nodes.back().point, goal_state.point)) {
        the_map->generate_points(1, the_map->width, the_map->height, 
                                 the_map->points[0].left_to_up.length, 
                                 the_map->points[0].left_to_right.length);
        extend(the_map, &kd, search, bmp);
    }
}

static
double get_phi(const Coordinates& one, const Coordinates& two)
{
    double catheter_1 = one.x - two.x;
    double catheter_2 = one.y - two.y;
    double hypotenuse = sqrt(pow(catheter_1, 2) + pow(catheter_2, 2));

    double asin_val = asin(catheter_2/hypotenuse) * 180 / M_PI;

    if (catheter_1 >= 0 && catheter_2 >= 0) { return asin_val; } 
    else if (catheter_1 <= 0 && catheter_2 >= 0) { return 180 - asin_val; } 
    else if (catheter_1 >= 0 && catheter_2 < 0) { return asin_val + 360; }
    else if (catheter_1 <= 0 && catheter_2 <= 0) { return 180 + abs(asin_val); }
}

void RrTree::extend(Map* the_map, KdTree * kd, bool search, Bitmap * bmp) 
{
    Coordinates new_point = the_map->points.back().left_to_right; // взяли только что сгенерир точку
    double best_distance = the_map->width * the_map->height;
    int best_index = -1;
    if (search) {
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

    if (!is_available(map, nodes[best_index].point, the_map->points.back())) {
        nodes.push_back(RrtNode(the_map->points.back(), best_index));
        kd->push(new_point.left_to_up, 0, -1, -1);
        nodes[best_index].children.push_back(nodes.size() - 1);
    } else {
        the_map->points.pop_back();
        return;
    }
}
/*
void RrTree::get_path(int index, double phi) 
{
    if (index == nodes.size() - 1) {
        path.push_back(goal_state.point);
        double new_phi = get_phi(nodes[index].point, goal_state.point) + 180;
        Coordinates point = nodes[index].point;
        point.phi = new_phi;
        path.push_back(point);

    } else {
        Coordinates point = nodes[index].point;
        point.phi = phi;
        path.push_back(point);
    }
    if (nodes[index].parent != -1) {
        double new_phi = get_phi(nodes[index].point, nodes[nodes[index].parent].point);
        get_path(nodes[index].parent, new_phi);
    }
}
*/
inline
double RrTree::get_distance(Coordinates point_1, Coordinates point_2) 
{
    return (point_1.x - point_2.x)*(point_1.x - point_2.x) +
           (point_1.y - point_2.y)*(point_1.y - point_2.y);
}
/*
void RrTree::go(int index) 
{
    edges.clear();
    
    Coordinates temp = nodes[index].point;

    if (nodes[index].children.size() != 0) {
        for (size_t i = 0; i < nodes[index].children.size(); i++) {
            this->edges.push_back(temp);
            this->edges.push_back(nodes[nodes[index].children[i]].point);
        }
    }
}
*/
/*
void RrTree::optimize_path(Map * map, int iter) 
{
    unsigned step = 2;
    while (iter) {
        std::vector<Coordinates> new_path;
        new_path.push_back(path.front());

        std::vector<Coordinates>::size_type index = 0;
        std::vector<Coordinates>::size_type end = path.size() - step - 1;
        //std::cout << "index " << index << " end " << end << "\n";
        while (index < end) {
            if (! is_available(map, path[index], path[index + step])){
                if (new_path.back() != path[index]) new_path.push_back(path[index]);
                new_path.push_back(path[index + step]);
                index += step + 1;
            } else {
                new_path.push_back(path[index]);
                index++;
            }

        }
        new_path.push_back(path.back());
        if (new_path.size() > 2) path = new_path;
        iter--;
    }
}
*/
bool RrTree::is_available(Map *the_map, Contour object_1, Contour object_2) 
{
    //сначала разворот первого обекта на нудный угол
    //потом кирпич до нового узла
    //  такой кирпич - это между left-to-right старым и новым
    //для первого жтапа достаточно проверить пиццу для гипотенузы 
    //+ пиццу для left-to-right + is_valid для конечно состояния поворота
    double end_x = object_1.left_to_up.x + object_1.left_to_up.length * 
                   cos(object_1.left_to_up.phi * M_PI / 180);
    double end_y = object_1.left_to_up.y + object_1.left_to_up.length *
                   sin(object_1.left_to_up.phi * M_PI / 180);
    end_x = end_x + object_1.left_to_right.length *
            cos(object_1.left_to_right.phi * M_PI / 180);
    end_y = end_y + object_1.left_to_right.length *
            sin(object_1.left_to_right.phi * M_PI / 180);
    //теперь вычислим угол и длину гипотенузы, чтобы можно было представить ее в виде
    //точки приложения, длины и угла
    Coordinates hypotenuse(object_1.left_to_up);
    hypotenuse.phi = get_phi(object_1.left_to_up, end_right_high);
    hypotenuse.length = sqrt(pow(object_1.left_to_up.x - end_x, 2) + 
                             pow(object_1.left_to_up.y - end_y, 2));
    //теперь у нас есть сама гипотенуза, все готово для того, чтобы начать проверять секторы круга
    //для этого нужен угол, на который должен развернуться первый объект, чтобы поехать ко второму
    double new_phi = get_phi(object_1.left_to_up, object_2.left_to_up);
    //теперь, прежде чем проверять пиццу, нужно проверить валидность состояния,
    //в которое приедт первый объект, развернувшись 
    { 
        Contour temp_contour_for_brick(Coordinates(object_2.left_to_up), new_phi, 
                                       object_1.left_to_up.length, object_1.left_to_right.length);
        if (the_map->is_valid(temp_contour_for_brick)) return true; 
    }
    //поворот возможен, значит проверяем секторы
    //создадим две темповые палки 
    Coordinates temp_for_hypotenuse = hypotenuse; 
    temp_for_hypotenuse.phi += new_phi;
    temp_for_hypotenuse.phi %= 360;
    Coordinates temp_for_left_to_right(object_1.left_to_right);
    temp_for_left_to_right.phi += new_phi;
    temp_for_left_to_right.phi %= 360;
    if (Check::check_slice(the_map, hypotenuse, temp_for_hypotenuse) || 
        Check::check_slice(the_map, object_1.left_to_right, temp_for_left_to_right)) return true;
    //теперь слайсы проверены, и поворот в сторону обекта_2 точно возможен
    //создадим контур обекта, который приехал в левую нижнюю точку объекта_2 
    //и чекнем его
    Contour temp_contour_for_brick(Coordinates(object_2.left_to_up), new_phi, 
                                               object_1.left_to_up.length, object_1.left_to_right.length);
    if (the_map->is_valid(temp_contour_for_brick)) return true;
    //оасталось проверить только путь от первого темпового объекта, до второго
    //это можно сделать, проверив кирпичи между left_to_right этих объектов
    if (Check::check_brick(the_map, temp_for_left_to_right, temp_contour_for_brick.left_to_right)) return true;

    //ну теперь вообще все достижимо
    return false;
}
