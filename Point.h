#ifndef GD_PROJECT_Point_H
#define GD_PROJECT_Point_H

struct Point {
    double x;
    double y;

    Point(double x_coord, double y_coord);
    Point(const Point & vec2_to_copy);
    Point operator=(const Point & other_vec);
    void save(const char * filename);
};


#endif //GD_PROJECT_Point_H
