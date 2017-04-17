#ifndef FEET_H
#define FEET_H

#include "Contour.h"

class Feet {
public:
    Contour left;
    Contour right;

    Feet(Contour left_, Contour right_) :
        left(left_),
        right(right_)
    {}
};

#endif
