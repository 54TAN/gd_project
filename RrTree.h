#ifndef GD_PROJECT_RRTREE_H
#define GD_PROJECT_RRTREE_H

#include "KdTree.h"
#include "RrtNode.h"
#include "Map.h"

struct RrTree {
    std::vector<RrtNode> nodes;
    std::vector<Point> path;
    std::vector<Point> edges;
    RrtNode goal_state;
    double min_distance;

    RrTree(Map* the_map, double distance, bool search); // 1 = kd
    bool is_available(Map* the_map, Point point_1, Point point_2);
    double get_distance(Point point_1, Point point_2);
    void extend(Map* the_map, KdTree * kd, bool search);
    void get_path(int index);
    void go(int index);
};


#endif //GD_PROJECT_RRTREE_H
