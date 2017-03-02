#ifndef GD_PROJECT_KDTREE_H
#define GD_PROJECT_KDTREE_H

#include <fstream>
#include <vector>

#include "KdNode.h"

struct KdTree {
public:
    //void input(const char * filename);
    void push(std::vector <double> temp, int index_vec, int index_dev, int index_par);
    //void save(std::ostream &output_file);
    double get_distance(std::vector <double> node_1, std::vector <double> node_2);
    void seek_nearest_with_kd(int index_root, std::vector <double> other_node_coordinates,
                              int index_dim, int & best_index, double & best_distance);
    /*void seek_nearest_linear(double * other_node_coordinates, int & best_index, double & best_distance);
    double * get_node(int index);
    size_t get_size();

    */
    std::vector <KdNode> nodes;
};


#endif //GD_PROJECT_KDTREE_H
