#include <cmath>
#include <iostream>

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
    if (!is_available(the_map, new_point, nodes[best_index].point)  &&
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

void RrTree::optimize_path(Map * map, int step, int iter) {

    while (iter) {
        for (int i = 0; i < path.size() - step; i++) {
            if (!is_available(map, path[i], path[i + step])) {
                path.erase(path.begin() + i + 1, path.begin() + i + step - 1);
            }
        }
        iter--;
    }
}

bool RrTree::is_available(Map *the_map, Coordinates object_1, Coordinates object_2) {

    if (Check::check_brick(the_map, object_1, object_2) || Check::check_slice(the_map, object_1, object_2))
        return true;
    else return false;

}


