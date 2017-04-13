#include <cmath>
#include <iostream>
#include <string>

#include "RrTree.h"
#include "Render.h"
#include "Geometry.h"
#include "Check.h"

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
    //еще одна точка, чтобы было действительно достижимо
    Coordinates extra_point = new_point;
    extra_point.phi = nodes[best_index].point.phi;
    if (!is_available(the_map, new_point, nodes[best_index].point)  &&
//        !is_available(the_map, extra_point, nodes[best_index].point) &&
        sqrt(best_distance) <= this->min_distance) {

        nodes.push_back(RrtNode(new_point, best_index));
        kd->push(new_point.coords, 0, -1, -1);
        nodes[best_index].children.push_back(nodes.size() - 1);

        if (bmp && counter % 10) {
            //out in .bmp
            for (size_t i = 0; i < nodes.size(); i++) {
                go(i);
                if (edges.size())
                    render_path(edges, bmp, 0);
            }
            std::string name = "bmp";
            name += std::to_string(counter);
            name += ".bmp";
            bmp->out_bmp(name.c_str());

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
        new_path.push_back(path[0]);

        std::vector<Coordinates>::size_type index = 0;
        std::vector<Coordinates>::size_type end = path.size() - step - 1;
        std::cout << "index " << index << " end " << end << "\n";
        while (index < end) {
            if (! is_available(map, path[index], path[index + step])){
                if (new_path.back() != path[index]) new_path.push_back(path[index]);
                new_path.push_back(path[index + step]);
                index += step;
            } else {
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
    //only for gif
    /*
    double * equation = new double[3];
    Geometry::get_equation(equation, object_1, object_2);
    std::cout << "equation " << equation[0] << " " << equation[1] << " " << equation[2] << "\n";
    double low_bound = std::min(object_1.y, object_2.y);
    double high_bound = std::max(object_1.y, object_2.y);
*/
    bool ** some;
    std::vector<Coordinates> coords_for_check;
    bresenham(some, object_1, object_2, 0, 0, 0, 0, &coords_for_check);
    //std::cout << "coords_for_check.size()" << coords_for_check.size() << "\n";
    for (auto item : coords_for_check) {
        if (the_map->is_point_in_obstacle(item)) {
            return true;
        }
    }
/*
    for (int i = low_bound; i < high_bound; i++) {
        double line[3] = {0, 1, (double) (-1 * (i))};
        double x;
        for (int j = std::min(object_1.x, object_2.x); j < std::max(object_2.x, object_1.x); j++) {
            std::cout << -i*equation[1] << " " << j*equation[0] + equation[2] << "\n" ;
            if (equation[1] == round(j*equation[0]/(-i) + (int)equation[2]/(-i))) {
                //std::cout << j << " " << i << "\n";
                if (the_map->is_point_in_obstacle(Coordinates(j, i))) {
                    return true;
                }
            }
        }
 */       /*
        if (Geometry::get_intersection(line, equation, &x)) {
            std::cout << x << " " << i << "\n";
            Coordinates pt(x, i); // запиливаем
            if (the_map->is_point_in_obstacle(pt)) {
                return true;
            } 
        }
        */
    //}
    return false;

/*
    if (Check::check_brick(the_map, object_1, object_2) || Check::check_slice(the_map, object_1, object_2))
        return true;
    else return false;
*/
}


