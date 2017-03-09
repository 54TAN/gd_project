#ifndef GD_PROJECT_RRTREE_H
#define GD_PROJECT_RRTREE_H

#include "KdTree.h"
#include "RrtNode.h"
#include "Map.h"
#include "Bitmap.h"

struct RrTree {
    std::vector<RrtNode> nodes;
    std::vector<Coordinates> path;
    std::vector<Coordinates> edges;
    RrtNode goal_state;
    double min_distance;

    RrTree(Map* the_map, double distance);
    RrTree(double distance);
    void search(Map* the_map, bool search, Bitmap * bmp = NULL); // search = 1 = kd
    bool is_available(Map* the_map, Coordinates point_1, Coordinates point_2);
    bool is_available_second(Map* the_map, Coordinates object_1, Coordinates object_2);
    double get_distance(Coordinates point_1, Coordinates point_2);
    void extend(Map* the_map, KdTree * kd, bool search, Bitmap * bmp = NULL);
    void get_path(int index);
    void go(int index);
    void optimize_path(Map * map, int step = 3, int iter = 2);
};



#endif //GD_PROJECT_RRTREE_H
