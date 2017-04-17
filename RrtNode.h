#ifndef GD_PROJECT_RRTNODE_H
#define GD_PROJECT_RRTNODE_H

#include <vector>
#include "Coordinates.h"
#include "Contour.h"

class RrtNode {
public:
    int parent;
    std::vector<int> children;
    Contour point;
    
    RrtNode(Coordinates, int parent = -1);
    void output();
    bool operator==(const RrtNode&);
};


#endif //GD_PROJECT_RRTNODE_H
