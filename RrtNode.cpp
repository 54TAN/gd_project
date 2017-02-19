#include <iostream>
#include "RrtNode.h"

RrtNode::RrtNode(GeomVector generated_point, int parent) :
        point(generated_point)
{
    this->parent = parent;
}

bool RrtNode::operator==(const RrtNode& right) {
    if (this->point.x == right.point.x && this->point.y == right.point.y) return true;
    else return false;
}

void RrtNode::output() {
    std::cout << point.x << " " << point.y << '\n';
}
