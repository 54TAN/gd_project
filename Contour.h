#ifndef CONTOUR_H
#define CONTOUR_H

#include "Coordinates.h"
#include "Bitmap.h"

#include <ostream>
//#include "Render.h"

class Contour {
public:
    Coordinates left_to_up;
    Coordinates left_to_right;

    Contour() = default;
    Contour(Coordinates, double, double, double); 
    Contour(const Contour &);
    Contour& operator=(const Contour &);
    bool operator==(const Contour &);
    bool operator!=(const Contour &);

    void redirect(double phi);
};

void render_contour(const Contour&, Bitmap*, bool = false);
std::ostream& operator<<(std::ostream&, Contour);

#endif
