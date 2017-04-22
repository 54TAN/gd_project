#include <cmath>
#include <iostream>
#include <vector>

#include "Contour.h"

#include "Render.h"

Contour::Contour(Coordinates point_,
                 double phi_, double width_, double height_) :
    left_to_up(point_),
    left_to_right(point_)
{
    left_to_up.phi = phi_;
    left_to_up.length = height_;
    left_to_right.length = width_;
    if (phi_ >= 90) left_to_right.phi = phi_ - 90;
    else left_to_right.phi = 270 + phi_;
}

void Contour::redirect(double phi)
{
    left_to_up.phi = phi;
    if (phi >= 90) left_to_right.phi = phi - 90;
    else left_to_right.phi = 270 + phi;
}

Contour::Contour(const Contour& other) :
    left_to_up(other.left_to_up),
    left_to_right(other.left_to_right)
{}

Contour& Contour::operator=(const Contour& other)
{
    if (this != & other) {
        left_to_up = other.left_to_up;
        left_to_right = other.left_to_right;
    }
}

bool Contour::operator==(const Contour& other)
{
    return left_to_up == other.left_to_up && left_to_right == other.left_to_right;
}

bool Contour::operator!=(const Contour& other)
{
    return !(*this == other);
}

void render_contour(const Contour& contour, Bitmap* bmp, bool color)
{
    std::vector<Coordinates> coords;

    double end_x = contour.left_to_up.x + contour.left_to_up.length * 
                   cos(contour.left_to_up.phi * M_PI / 180);
    double end_y = contour.left_to_up.y + contour.left_to_up.length *
                   sin(contour.left_to_up.phi * M_PI / 180);
    Coordinates end_left_high(end_x, end_y);
    coords.push_back(contour.left_to_up);
    coords.emplace_back(end_x, end_y);
    render_path(coords, bmp, color);

    coords.clear();
    end_x = contour.left_to_right.x + contour.left_to_right.length * 
            cos(contour.left_to_right.phi * M_PI / 180);
    end_y = contour.left_to_right.y + contour.left_to_right.length *
            sin(contour.left_to_right.phi * M_PI / 180);
    coords.push_back(contour.left_to_right);
    Coordinates end_right_low(end_x, end_y);
    coords.emplace_back(end_x, end_y);
    render_path(coords, bmp, color);

    coords.clear();
    end_x = end_left_high.x + contour.left_to_right.length *
            cos(contour.left_to_right.phi * M_PI / 180);
    end_y = end_left_high.y + contour.left_to_right.length * 
            sin(contour.left_to_right.phi * M_PI / 180);
    Coordinates end_right_high(end_x, end_y);
    coords.push_back(end_left_high);
    coords.push_back(end_right_high);
    render_path(coords, bmp, color);

    coords.clear();
    coords.push_back(end_right_low);
    coords.push_back(end_right_high);
    render_path(coords, bmp, color);
}
