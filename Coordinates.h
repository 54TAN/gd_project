#ifndef GD_PROJECT_Point_H
#define GD_PROJECT_Point_H

#include <vector>

class Coordinates {
public:
    double x;
    double y;
    double phi;
    double length;

    std::vector <double> coords;

    Coordinates() = default;
    Coordinates(double, double);
    Coordinates(double, double, double, double);
    Coordinates(const Coordinates &);
    Coordinates& operator=(const Coordinates &);

    void save(const char *);

    bool operator==(Coordinates);
    bool operator!=(Coordinates);
};


#endif //GD_PROJECT_Point_H
