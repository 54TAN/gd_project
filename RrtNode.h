#ifndef GD_PROJECT_RRTNODE_H
#define GD_PROJECT_RRTNODE_H

#include <vector>
#include "GeomVector.h"

struct RrtNode {
    int parent;
    std::vector<int> children;
    GeomVector point;

    RrtNode(GeomVector generated_point, int parent = -1);
    void output();
    bool operator==(const RrtNode& right);
};


#endif //GD_PROJECT_RRTNODE_H
