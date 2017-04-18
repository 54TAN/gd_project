#ifndef GD_PROJECT_RRTREE_H
#define GD_PROJECT_RRTREE_H

#include "KdTree.h"
#include "RrtNode.h"
#include "Map.h"
#include "Bitmap.h"
#include "Contour.h"

struct RrTree {
    std::vector<RrtNode> nodes;
    std::vector<Contour> path;
    std::vector<Contour> edges;
    RrtNode goal_state;
    double min_distance;

    RrTree(Map* the_map, double distance);
    RrTree(double distance);
    void search(Map* the_map, bool search, Bitmap * bmp = NULL); // search = 1 = kd
    bool is_available(Map* the_map, Contour object_1, Contour object_2, Bitmap * = NULL);
    inline double get_distance(Coordinates point_1, Coordinates point_2);
    void extend(Map* the_map, KdTree * kd, bool search, Bitmap * bmp = NULL);
    void get_path(int index, double phi = 0);
    void go(int index);
    void optimize_path(Map * map, int iter = 2);
};



#endif //GD_PROJECT_RRTREE_H
