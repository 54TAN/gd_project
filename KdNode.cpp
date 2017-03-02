#include "KdNode.h"
#include "Coordinates.h"

#include <algorithm>
#include <iterator>

KdNode::KdNode(std::vector<double> * point, int index):
        left(-1),
        right(-1),
        split_index(index)
{
    if (!point) {
        std::fill(coords.begin(), coords.end(), 0);
    } else {
        coords = *point;
    }
}