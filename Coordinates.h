#ifndef GD_PROJECT_Point_H
#define GD_PROJECT_Point_H

#include <vector>

struct Coordinates {
    double x;
    double y;
    double phi;
    double length;

    std::vector <double> coords;

    Coordinates(double x, double y);
    Coordinates(double x_coord, double y_coord, double phi, double length);
    Coordinates(const Coordinates & other_vec);
    Coordinates& operator=(const Coordinates & other_vec);
    void save(const char * filename);
    bool operator==(Coordinates other);
};


#endif //GD_PROJECT_Point_H
