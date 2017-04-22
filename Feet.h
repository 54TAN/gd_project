#ifndef FEET_H
#define FEET_H

#include "Contour.h"

class Feet {
public:
    Contour left;
    Contour right;

    Feet() = default;

    Feet(Contour left_, Contour right_) :
        left(left_),
        right(right_)
    {}

    Feet(const Contour&, bool = false);
};

void render_feet(const Feet&, Bitmap*, bool = false);

#endif
