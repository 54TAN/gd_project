#ifndef GD_PROJECT_KDNODE_H
#define GD_PROJECT_KDNODE_H

#include "Consts.h"

struct KdNode {
    double value[N];
    int split_index;
    int left;
    int right;

    KdNode(double * values = 0, int index = 0);
};


#endif //GD_PROJECT_KDNODE_H
