#include <cmath>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <iterator>
#include <functional>

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
    kd.nodes.push_back(KdNode(&the_map->points.front().left_to_up.coords, 0));
	long long counter = 0;
    while (is_available(the_map, nodes.back().point, goal_state.point, bmp)) {
        the_map->generate_points(1, the_map->width, the_map->height, 
                                 the_map->points[0].left_to_right.length, 
                                 the_map->points[0].left_to_up.length);
		counter++;
        extend(the_map, &kd, search, bmp);
    }
	std::cout << "amount of generated points : " << counter << "\n";
}

static
double get_phi(const Coordinates& one, const Coordinates& two)
{
    double catheter_1 = two.x - one.x;
    double catheter_2 = two.y - one.y;
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
            double tmp = get_distance(new_point, nodes[i].point.left_to_up);
            if (tmp < best_distance) {
                best_index = i;
                best_distance = tmp;
            }
        }
    }

    if (!is_available(the_map, nodes[best_index].point, the_map->points.back(), bmp)) {
        nodes.push_back(RrtNode(the_map->points.back(), best_index));
        kd->push(new_point.coords, 0, -1, -1);
        nodes[best_index].children.push_back(nodes.size() - 1);
    } else {
        the_map->points.pop_back();
        return;
    }
}

void RrTree::get_path(int index, double phi) 
{
    if (index == nodes.size() - 1) {
        path.push_back(goal_state.point);
        double new_phi = get_phi(nodes[index].point.left_to_up, goal_state.point.left_to_up);
        Contour point = nodes[index].point;
        point.redirect(new_phi);
        path.push_back(point);
        //path.push_back(nodes[index].point);

    } else {
        Contour point = nodes[index].point;
        point.redirect(phi);
        path.push_back(point);
        //path.push_back(nodes[index].point);
    }
    if (nodes[index].parent != -1) {
        double new_phi = 180 + get_phi(nodes[index].point.left_to_up, 
                                 nodes[nodes[index].parent].point.left_to_up);
        get_path(nodes[index].parent, new_phi);
        //get_path(nodes[index].parent, 0);
    }
}

inline
double RrTree::get_distance(Coordinates point_1, Coordinates point_2) 
{
    return (point_1.x - point_2.x)*(point_1.x - point_2.x) +
           (point_1.y - point_2.y)*(point_1.y - point_2.y);
}

void RrTree::go(int index) 
{
    edges.clear();
    
    Coordinates temp = nodes[index].point.left_to_right;

    if (nodes[index].children.size() != 0) {
        for (size_t i = 0; i < nodes[index].children.size(); i++) {
            this->edges.push_back(temp);
            this->edges.push_back(nodes[nodes[index].children[i]].point.left_to_right);
        }
    }
}

void RrTree::optimize_path(Map * map, int iter) 
{
    unsigned step = 2;
    iter = 10;
    if (path.size() != 2)
    while (iter) {
        std::vector<Contour> new_path;
        new_path.push_back(path[0]);
        std::vector<Contour>::size_type index = 1;
        std::vector<Contour>::size_type end = path.size() - step - 1;
        //std::cout << "index " << index << " end " << end << "\n";
        while (index <= path.size() - 1) {
            if (index <= end && ! is_available(map, path[index], path[index + step])){
                if(new_path.back() != path[index]) 
                    new_path.push_back(path[index]);
                new_path.push_back(path[index + step]);
                index += step + 1;
            } else {
                new_path.push_back(path[index]);
                index++;
            }

        }
        //new_path.push_back(path[path.size() - 1]);
        if (new_path.size() > 2) {
            path.clear();
            //std::cout << path.size() << " ";
            std::copy(new_path.begin(), new_path.end(), std::back_inserter(path));
            new_path.clear();
            //std::cout << path.size() << "\n";
        } //path = new_path;
        iter--;
    }
    //std::cout << "one\n";
    //for (auto item : path) std::cout << item.left_to_up.phi << " ";
    //std::cout << "\n";

    std::vector<Contour> path_redirected;
    path_redirected.push_back(std::ref(map->points.front()));
    for (size_t i = 0; i < path.size() - 1; i++) {
        Contour temp = path[i];
        //std::cout << "path[i].phi = " << path[i].left_to_up.phi << "; ";
        double new_phi = get_phi(path[i].left_to_up, path[i + 1].left_to_up);
        //std::cout << "new_phi = " << new_phi << "\n";
        temp.redirect(new_phi);
        path_redirected.push_back(temp);
        temp = path[i + 1];
        temp.redirect(new_phi);
        path_redirected.push_back(temp);
    }
    path_redirected.push_back(std::ref(path.back()));
    path.clear();
    std::copy(path_redirected.begin(), path_redirected.end(), 
              std::back_inserter(path));

    //std::cout << "two\n\t";
    //for (auto item : path) std::cout << item.left_to_up.phi << "\n\t";
    //std::cout << "\n";

    //теперь навставляем еще квадратов
    //std::cout << "path was optimized but not quite " << path.size() << " <- current size\n";
    std::vector<Contour> final_path;
    final_path.push_back(std::ref(path.front()));
    for (std::vector<Contour>::size_type i = 1; 
         i < path.size() - 1; i++) 
    {
        //вычислим конец to_up
        //если расстояние от этого конца до начала следующего узла меньше длины to_up
        //то пушим состояние, которое начинается с этого самого конца
        //с такими же углами
        //если меньше, то тоже пушим, но выставляем к-н флаг, что так делать больше нельзя
        //потом в пути идут два узла с одинаковой левой пяткой
        //воспользуемся этим и перепрыгнем через один такой узел
        //повторим процедуру
       // std::cout << "in here\n";
        if (path[i].left_to_right.x == path[i + 1].left_to_up.x &&
            path[i].left_to_right.y == path[i + 1].left_to_up.y) {
            final_path.push_back(path[i]);
            continue;
        }
        final_path.push_back(path[i]);
        
        while (true) {
            double end_x = final_path.back().left_to_up.x + final_path.back().left_to_up.length * 
                           cos(final_path.back().left_to_up.phi * M_PI / 180);
            double end_y = final_path.back().left_to_up.y + final_path.back().left_to_up.length *
                           sin(final_path.back().left_to_up.phi * M_PI / 180);
            Coordinates current_end(end_x, end_y);
            //std::cout << end_x << " - " << end_y << "\n";
            double distance = get_distance(current_end, path[i + 1].left_to_up);
            //std::cout << path[i].left_to_up.length << "\n";
            //std::cout << sqrt(distance) << "\n";
            if (sqrt(abs(distance)) > path[i].left_to_up.length) {
                final_path.emplace_back(current_end, 
                                        path[i].left_to_up.phi,
                                        path[i].left_to_right.length,
                                        path[i].left_to_up.length);
            } 
            if (sqrt(distance) <= path[i].left_to_up.length) {
                final_path.emplace_back(current_end, 
                                        path[i].left_to_up.phi,
                                        path[i].left_to_right.length,
                                        path[i].left_to_up.length);
                break;
            }

        }
        
    }

    final_path.push_back(std::ref(path.back()));
    path.clear();
    std::copy(final_path.begin(), final_path.end(), 
              std::back_inserter(path));
}

static
Coordinates get_hypotenuse(const Contour& object_1)
{
    double end_x = object_1.left_to_up.x + object_1.left_to_up.length * 
                   cos(object_1.left_to_up.phi * M_PI / 180);
    double end_y = object_1.left_to_up.y + object_1.left_to_up.length *
                   sin(object_1.left_to_up.phi * M_PI / 180);
    end_x = end_x + object_1.left_to_right.length *
            cos(object_1.left_to_right.phi * M_PI / 180);
    end_y = end_y + object_1.left_to_right.length *
            sin(object_1.left_to_right.phi * M_PI / 180); // right
    //теперь вычислим угол и длину гипотенузы, чтобы можно было представить ее в виде
    //точки приложения, длины и угла
    //std::cout << object_1.left_to_up.x << " " << object_1.left_to_up.y << "\n";
    Coordinates hypotenuse(object_1.left_to_up);
    hypotenuse.phi = get_phi(object_1.left_to_up, Coordinates(end_x, end_y));
    hypotenuse.length = sqrt(pow(object_1.left_to_up.x - end_x, 2) + 
                             pow(object_1.left_to_up.y - end_y, 2));
    return hypotenuse; 
}

bool RrTree::is_available(Map *the_map, Contour object_1, Contour object_2, Bitmap * bmp) 
{
    //сначала разворот первого обекта на нудный угол
    //потом кирпич до нового узла
    //  такой кирпич - это между left-to-right старым и новым
    //для первого жтапа достаточно проверить пиццу для гипотенузы 
    //+ пиццу для left-to-right + is_valid для конечно состояния поворота
    Coordinates hypotenuse = get_hypotenuse(object_1);
    Coordinates second_hypotenuse;
    Coordinates second_left_to_right;
    //теперь у нас есть сама гипотенуза, все готово для того, чтобы начать проверять секторы круга
    //для этого нужен угол, на который должен развернуться первый объект, чтобы поехать ко второму
    double new_phi = get_phi(object_1.left_to_up, object_2.left_to_up);
    
    //теперь, прежде чем проверять пиццу, нужно проверить валидность состояния,
    //в которое приедт первый объект, развернувшись 
	//auto start_time = std::chrono::system_clock::now();
	//auto end_time = std::chrono::system_clock::now();
    { 
        Contour temp_contour_for_brick(Coordinates(object_1.left_to_up), new_phi, 
                                       object_1.left_to_right.length, object_1.left_to_up.length);
        //std::cout << temp_contour_for_brick.left_to_up.x << " " << temp_contour_for_brick.left_to_up.y << "\n";
        //std::cout << "in here\n";
        //render_contour(temp_contour_for_brick, bmp);
        second_hypotenuse = get_hypotenuse(temp_contour_for_brick);
        second_left_to_right = temp_contour_for_brick.left_to_right;
        
       	//start_time = std::chrono::system_clock::now(); 
        if (the_map->is_valid(temp_contour_for_brick)) return true; 
       	//end_time = std::chrono::system_clock::now(); 
    }

	//auto elapsed_brick = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    //поворот возможен, значит проверяем секторы
	//start_time = std::chrono::system_clock::now(); 
	//if (check_brick()) return true;
	//if (Check::check_slice(the_map, hypotenuse, second_hypotenuse)) return true;
    //if (Check::check_slice(the_map, object_1.left_to_right, second_left_to_right)) return true;
    
    //end_time = std::chrono::system_clock::now(); 
	//auto elapsed_slice = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	//std::cout << "slice's time : " << elapsed_slice.count() << "ms \n";
    //теперь слайсы проверены, и поворот в сторону обекта_2 точно возможен
    //создадим контур обекта, который приехал в левую нижнюю точку объекта_2 
    //и чекнем его
    Contour temp_contour_for_brick(Coordinates(object_2.left_to_up), new_phi, 
                                   object_1.left_to_right.length, object_1.left_to_up.length);
	//start_time = std::chrono::system_clock::now();
    if (the_map->is_valid(temp_contour_for_brick)) return true;
    //оасталось проверить только путь от первого темпового объекта, до второго
    //это можно сделать, проверив кирпичи между left_to_right этих объектов
    if (Check::check_brick(the_map, second_left_to_right, temp_contour_for_brick.left_to_right)) return true;
	//end_time = std::chrono::system_clock::now();
	//elapsed_brick += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	//std::cout << "brick's time : " << elapsed_brick.count() << "\n";
    //ну теперь вообще все достижимо
    return false;
}
