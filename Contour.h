#ifndef CONTOUR_H
#define CONTOUR_H

#include "Coordinates.h"
#include "Bitmap.h"
//#include "Render.h"

class Contour {
public:
    Coordinates left_to_up;
    Coordinates left_to_right;

    Contour(Coordinates, double, double, double); 
};

void render_contour(const Contour&, Bitmap*);

#endif
