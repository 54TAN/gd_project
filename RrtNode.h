#ifndef GD_PROJECT_RRTNODE_H
#define GD_PROJECT_RRTNODE_H

#include <vector>
#include "Coordinates.h"

struct RrtNode {
    int parent;
    std::vector<int> children;
    Coordinates point;

    RrtNode(Coordinates generated_point, int parent = -1);
    void output();
    bool operator==(const RrtNode& right);
};


#endif //GD_PROJECT_RRTNODE_H
