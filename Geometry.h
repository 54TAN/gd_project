#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Coordinates.h"

class Geometry {
public:
	static bool get_intersection_circle(double * coords, Coordinates circle, std::pair <Coordinates *, Coordinates *> pair);
	static void up_right_left(int &first, int &second, int intersections, int i, int end_left_y, int end_right_y, bool right);
	static bool get_intersection(double * firstLineCoefs, double * secondLineCoefs, double * x); 
	static void get_equation(double * coefs, Coordinates one, Coordinates two);
};

#endif
