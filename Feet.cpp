#include "Feet.h"
#include "Coordinates.h"

#include <cmath>

Feet::Feet(const Contour &border, bool special) 
{
    double for_left_heel_x, for_left_heel_y;
    if (special) {
        for_left_heel_x = 0;
        for_left_heel_y = 0;
    } else {
        for_left_heel_x = border.left_to_up.length / 2 * cos(border.left_to_up.phi * M_PI / 180);
        for_left_heel_y = border.left_to_up.length / 2 * sin(border.left_to_up.phi * M_PI / 180);
    }
    double end_x = border.left_to_up.x + for_left_heel_x;
    double end_y = border.left_to_up.y + for_left_heel_y;
    left = Contour(Coordinates(end_x, end_y), border.left_to_up.phi,
                   border.left_to_right.length / 2.5, border.left_to_up.length / 2.5);
    end_x = border.left_to_right.x +
            border.left_to_right.length / 2 *
            cos(border.left_to_right.phi * M_PI / 180);
    end_y = border.left_to_up.y +
            border.left_to_right.length / 2 *
            sin(border.left_to_right.phi * M_PI / 180);
    right = Contour(Coordinates(end_x, end_y), border.left_to_up.phi,
                    border.left_to_right.length / 2.5, border.left_to_up.length / 2.5);
}

void render_feet(const Feet &feet, Bitmap *bmp, bool color) 
{
    render_contour(feet.left, bmp, false);
    render_contour(feet.right, bmp, false);
}

std::ostream& operator<<(std::ostream & os, Feet feet)
{
    os << feet.left << feet.right;
    return os;
}
