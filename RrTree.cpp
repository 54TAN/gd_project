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

void RrTree::search(Map* the_map, bool search, Bitmap * bmp) {
    //double temp[2] = {the_map->points.front().x, the_map->points.front().y};
    KdTree kd;
    kd.nodes.push_back(KdNode(&the_map->points.front().coords, 0));
    //std::cout << nodes.size() << std::endl;
    while (is_available(the_map, nodes.back().point, goal_state.point)) {


        the_map->generate_points(1, the_map->width, the_map->height, the_map->points[0].length);
        //std::cout << nodes.size() << std::endl;
        /*std::cout << nodes.back().point.x << " "
                  << nodes.back().point.y << " "
                  << nodes.back().point.phi << "\n";
        */
        extend(the_map, &kd, search, bmp);
        //std::cout //<< nodes.back().point.x << " "
                  //<< nodes.back().point.y << " ";
                  //<< nodes.back().point.phi << "\n";
        ///std::cout << nodes.size() << std::endl;
    }/*
    get_path(nodes.size() - 1);
    render_path(path, bmp, 1);
    bmp->out_bmp("MAP_PATH.bmp");*/
}

static
double get_phi(const Coordinates& one, const Coordinates& two)
{
    double catheter_1 = one.x - two.x;
    double catheter_2 = one.y - two.y;
    double hypotenuse = sqrt(pow(catheter_1, 2) + pow(catheter_2, 2));

    std::cout << "catheter_1 " << catheter_1;
    std::cout << ", catheter_2 " << catheter_2;
    std::cout << ", hypotenuse " << hypotenuse;

    double asin_val = asin(catheter_2/hypotenuse) * 180 / M_PI;

    std::cout << ", asin_val = " << asin_val;// << " ";
    
    if (catheter_1 >= 0 && catheter_2 >= 0) {
        std::cout << ", returned " << asin_val << "\n";
        return asin_val;
    } else if (catheter_1 <= 0 && catheter_2 >= 0) {
        std::cout << ", returned " << 180 - asin_val << "\n";
        return 180 - asin_val; 
    } else if (catheter_1 >= 0 && catheter_2 < 0) {
        std::cout << ", returned " << asin_val + 360 << "\n";
        return asin_val + 360; 
    }
    else if (catheter_1 <= 0 && catheter_2 <= 0) {
        std::cout << ", returned " << 180 + abs(asin_val) << " " << "\n";
        return 180 + abs(asin_val);
    }
}

void RrTree::extend(Map* the_map, KdTree * kd, bool search, Bitmap * bmp) 
{
    Coordinates new_point = the_map->points.back(); // взяли только что сгенерир точку
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
    Coordinates point_from_rrt = nodes[best_index].point; // now we have two points for check them
    double new_phi = get_phi(new_point, point_from_rrt); // угол, который направляет палку в сторону новой точки
    /*
     * сначала проверяем кусок пиццы, который нужен, чтобы развернуться к новой точке
     * потом провепяем кирпич, который есть проезжемый путь к новой точке
     * в случае палки - это тоже палка, просто длинная
     * => генерить фи вообще не надо в карте
     * => добавить в чек функцию проверки палки
     *
     * */
    //еще одна точка, чтобы было действительно достижимо
    //first of all lets check a pizza
    Coordinates for_slice_checking = point_from_rrt;
    for_slice_checking.phi = new_phi;
    assert(point_from_rrt.x == for_slice_checking.x);

    if (!Check::check_slice(the_map, point_from_rrt, for_slice_checking) &&//checking if slice was nice
        //std::cout << "help yourself\n";
        !Check::check_line(the_map, point_from_rrt, new_point)) { // chekin if line was nice
            //сделать проверку на корректность пришедшей палки в новую точку
            new_point.phi = new_phi;
            if (!the_map->is_valid(new_point)) {
                std::cout << "**************** " << new_phi << "\n";
                nodes.push_back(RrtNode(new_point, best_index));
                kd->push(new_point.coords, 0, -1, -1);
                nodes[best_index].children.push_back(nodes.size() - 1);
                nodes[best_index].point.phi = new_phi;
            } else {
                the_map->points.pop_back();
                return;
            }
    } else {
        the_map->points.pop_back();
        return;
    }
/*
    
    Coordinates extra_point = new_point;
    extra_point.phi = nodes[best_index].point.phi;

    if (!is_available(the_map, new_point, nodes[best_index].point)  &&
        !is_available(the_map, extra_point, nodes[best_index].point) &&
        sqrt(best_distance) <= this->min_distance) 
    {
        nodes.push_back(RrtNode(new_point, best_index));
        kd->push(new_point.coords, 0, -1, -1);
        nodes[best_index].children.push_back(nodes.size() - 1);
    } else {
        the_map->points.pop_back();
        return;
    }
    */
}

void RrTree::get_path(int index, double phi) {
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

inline
double RrTree::get_distance(Coordinates point_1, Coordinates point_2) {
    return (point_1.x - point_2.x)*(point_1.x - point_2.x) +
           (point_1.y - point_2.y)*(point_1.y - point_2.y);

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

void RrTree::optimize_path(Map * map, int iter) 
{
    unsigned step = 2;
    /*
    while (iter) {
        auto beg = path.begin();
        for (;beg != path.end() - step;) {
            if (! is_available(map, *beg, *(beg + step))) {
                beg = path.erase(beg + 1);
            } else {
                beg++;
            }
        }
        iter--;
    }
    */
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

bool RrTree::is_available(Map *the_map, Coordinates object_1, Coordinates object_2) 
{
    //only for gif1
    /*
    bool ** some;
    std::vector<Coordinates> coords_for_check;
    bresenham(some, object_1, object_2, 0, 0, 0, 0, &coords_for_check);
    //std::cout << "coords_for_check.size()" << coords_for_check.size() << "\n";
    for (auto item : coords_for_check) {
        if (the_map->is_point_in_obstacle(item)) {
            return true;
        }
    }
    */

    if (Check::check_brick(the_map, object_1, object_2) || Check::check_slice(the_map, object_1, object_2))
        return true;
    else return false;
}


