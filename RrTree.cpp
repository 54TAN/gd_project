#include <cmath>
#include <iostream>
#include "RrTree.h"
#include "Render.h"

RrTree::RrTree(Map* the_map, double distance, bool search, Bitmap * bmp) :
        goal_state(RrtNode(the_map->points.back())),
        min_distance(distance)
{
    nodes.push_back(RrtNode(the_map->points.front()));
    double temp[2] = {the_map->points.front().x, the_map->points.front().y};
    KdTree kd;
    kd.nodes.push_back(KdNode(temp, 0));
    while (!is_available(the_map, nodes.back().point, goal_state.point)) {
        the_map->generate_points(1, the_map->width, the_map->height);
        this->extend(the_map, &kd, search, bmp);
    }
    get_path(nodes.size() - 1);
    render_path(path, bmp, 1);
    bmp->out_bmp("MAP_PATH.bmp");
}

void RrTree::extend(Map* the_map, KdTree * kd, bool search, Bitmap * bmp)
{
    static int counter = 0;
    static int current = 0;

    Point new_point = the_map->points.back();
    double temp[2] = {new_point.x, new_point.y};
    double best_distance = the_map->width * the_map->height;
    int best_index = -1;
    if (search) {
        kd->seek_nearest_with_kd(0, temp, 0, best_index, best_distance);
    } else {
        for (size_t i = 0; i < nodes.size(); i++) {
            double tmp = get_distance(new_point, nodes[i].point);
            if (tmp < best_distance) {
                best_index = i;
                best_distance = tmp;
            }
        }
    }

    if ((this->is_available(the_map, new_point, nodes[best_index].point))  &&
        sqrt(best_distance) <= this->min_distance) {
        nodes.push_back(RrtNode(new_point, best_index));
        kd->push(temp, 0, -1, -1);
        nodes[best_index].children.push_back(nodes.size() - 1);

        //out in .bmp
        for (size_t i = 0; i < nodes.size(); i++) {
            go(i);
            if (edges.size())
                render_path(edges, bmp, 0);
        }

        //info to cmd
        counter++;
        if (counter % 50) {
            if (this->nodes.back().point.x > current) {
                current = this->nodes.back().point.x;
                std::cout << current / 10.0 << std::endl;
            }
        }
        bmp->out_bmp("MAP_PATH.bmp");
    } else {
        the_map->points.pop_back();
        return;
    }
}

void RrTree::get_path(int index)
{
    if (index == nodes.size() - 1) {
        path.push_back(goal_state.point);
    }
    path.push_back(nodes[index].point);
    if (nodes[index].parent != -1) {
        get_path(nodes[index].parent);
    }
}

double RrTree::get_distance(Point point_1, Point point_2)
{
    return (point_1.x - point_2.x)*(point_1.x - point_2.x) +
           (point_1.y - point_2.y)*(point_1.y - point_2.y);

}

bool RrTree::is_available(Map* the_map, Point point_1, Point point_2)
{
    if (the_map->obstacles.size() == 0) {
        return true;
    }

    bool ** temp_plain = new bool * [the_map->width];
    for (size_t i = 0; i < the_map->width; i++) {
        temp_plain[i] = new bool [the_map->height];
    }
    for (size_t i = 0; i < the_map->width; i++) {
        for (size_t j = 0; j < the_map->height; j++) {
            temp_plain[i][j] = false;
        }
    }

    bresenham(temp_plain, point_1, point_2, 0);

    for (size_t i = 0; i < the_map->width; i++) {
        for (size_t j = 0; j < the_map->height; j++) {
            if (temp_plain[i][j]) {
                Point pt((double)i, (double)j);
                if (!the_map->is_point_in_obstacle(pt)) {
                    for (size_t k = 0; k < the_map->width; k++) {
                        delete [] temp_plain[k];
                    }
                    delete temp_plain;
                    //std::cout << "YOU SHELL NOT PAAAAASS!!!" << std::endl;
                    return false;
                }
            }
        }
    }

    for (size_t k = 0; k < the_map->width; k++) {
        delete [] temp_plain[k];
    }
    delete temp_plain;

    return true;
}

void RrTree::go(int index)
{
    edges.clear();
    
    Point temp = nodes[index].point;

    if (nodes[index].children.size() != 0) {
        for (size_t i = 0; i < nodes[index].children.size(); i++) {
            this->edges.push_back(temp);
            this->edges.push_back(nodes[nodes[index].children[i]].point);
        }
    }
}
