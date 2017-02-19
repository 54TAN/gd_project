#ifndef GD_PROJECT_GEOMVECTOR_H
#define GD_PROJECT_GEOMVECTOR_H


struct GeomVector {
    double x;
    double y;

    GeomVector(double x_coord, double y_coord);
    GeomVector(const GeomVector & vec2_to_copy);
    GeomVector operator=(const GeomVector & other_vec);
    void save(const char * filename);
};


#endif //GD_PROJECT_GEOMVECTOR_H
