#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Coordinates.h"

class Geometry {
public:
	static bool get_intersection_circle(double*, Coordinates, std::pair<Coordinates*, Coordinates*>);
	static void up_right_left(int&, int&, int, int, int, int, bool);
	static bool get_intersection(double*, double*, double*); 
	static void get_equation(double*, Coordinates, Coordinates);
};

#endif
