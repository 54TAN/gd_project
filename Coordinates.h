#ifndef GD_PROJECT_Point_H
#define GD_PROJECT_Point_H

#include <vector>

struct Coordinates {
    double x;
    double y;
    double phi;
    double length;

    std::vector <double> coords;

    Coordinates(double x_coord, double y_coord, double phi = 60, double length = 60);
    Coordinates(const Coordinates & other_vec);
    Coordinates& operator=(const Coordinates & other_vec);
    void save(const char * filename);
};


#endif //GD_PROJECT_Point_H
