#include <iostream>
#include "RrtNode.h"

RrtNode::RrtNode(Contour generated_point, int parent_)
{
    parent = parent_;
    point.left_to_up = generated_point.left_to_up;
    point.left_to_right = generated_point.left_to_right;
}

bool RrtNode::operator==(const RrtNode& right) 
{
    if (point.left_to_up == right.point.left_to_up && 
        point.left_to_right == right.point.left_to_right) return true;
    else return false;
}
/*
void RrtNode::output() 
{
    std::cout << point.x << " " << point.y << '\n';
}
*/

