#ifndef GD_PROJECT_KDNODE_H
#define GD_PROJECT_KDNODE_H

#include "Consts.h"
#include "Coordinates.h"

class KdNode {
public:
    std::vector <double> coords;
    int split_index;
    int left;
    int right;

    KdNode(std::vector <double> * = nullptr, int = 0);
};


#endif //GD_PROJECT_KDNODE_H
