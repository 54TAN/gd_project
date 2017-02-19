#include "KdNode.h"
#include "KdTree.h"

#include <iostream>

void KdTree::input(const char * filename)
{
    std::ifstream input_file(filename);

    double temp[N] = {0};

    for (std::size_t j = 0; j < N; j++) {
        input_file >> temp[j];
    }
    nodes.insert(nodes.end(), KdNode(temp, 0));

    while (input_file >> temp[0]) {
        for (std::size_t j = 1; j < N; j++) {
            input_file >> temp[j];
        }
        push(temp, 0, -1, -1);
    }

    input_file.close();
}

void KdTree::push(double * temp, int index_vec, int index_dev, int index_par)
{
    /** next split axis */
    index_dev = (index_dev + 1) % N;

    if (index_vec == -1) {
        index_vec = nodes.size();
        nodes.insert(nodes.end(), KdNode(temp, index_dev));
        KdNode & n = nodes[index_par];
        if (nodes[index_vec].value[n.split_index] >= n.value[n.split_index]) {
            n.right = index_vec;
        } else {
            n.left = index_vec;
        }
    } else {
        KdNode & n = nodes[index_vec];
        double value = temp[index_dev];
        int next = (value >= n.value[n.split_index])
                   ? n.right
                   : n.left;

        push(temp, next, index_dev, index_vec);
    }
}

static
std::string get_info(const KdNode & n)
{
    std::string info;
    for (size_t i = 0 ; i < N ; i++) {
        info += ((i > 0) ? " " : "") + std::to_string((int)n.value[i]);
    }
    return info;
}

static
void save_link(std::ostream &output_file, const std::string & color, int parent_index, int child_index)
{
    if (child_index == -1 || parent_index == -1)
    {
        return;
    }

    output_file << "\tN" << parent_index << " -> ";
    output_file  << "N" << child_index << "[color=" << color << ",penwidth=3.0]" << ";" << std::endl;
}

void KdTree::save(std::ostream &output_file)
{
    /* save node IDs */
    output_file << "digraph {" << std::endl;
    output_file << "# Nodes:" << std::endl;

    std::size_t sz = nodes.size();
    for (std::size_t i = 0 ; i < sz ; i++)
    {
        output_file << "\tN" << i << " [label=\"" << get_info(nodes[i]) << "\"];"<< std::endl;
    }

    output_file << "# Links:" << std::endl;

    /* save links between nodes */
    for (std::size_t i = 0 ; i < sz ; i++)
    {
        save_link(output_file, "blue", i, nodes[i].left);
        save_link(output_file, "red",  i, nodes[i].right);
    }

    output_file << "}" << std::endl;
}

double KdTree::get_distance(double * node_1, double * node_2)
{
    double dist = 0, tmp;
    for (size_t i = 0; i < N; i++) {
        tmp = node_1[i] - node_2[i];
        dist += tmp * tmp;
    }
    return dist;
}

void KdTree::seek_nearest_with_kd(int index_root, double * other_node_coordinates,
                                   int index_dev, int & best_index, double & best_distance)
{
    if (index_root == -1) return;

    KdNode & n = nodes[index_root]; // current node in the tree

    double distance = get_distance(n.value, other_node_coordinates);
    double temp_direction = n.value[n.split_index] - other_node_coordinates[index_dev];
    double temp_2 = temp_direction*temp_direction;

    if (best_index == -1 || distance < best_distance) {
        best_distance = distance;
        best_index = index_root;
        if (!best_distance) return;
    }

    index_dev = (index_dev + 1) % N;
    seek_nearest_with_kd(temp_direction > 0 ? n.left : n.right,
                         other_node_coordinates, index_dev, best_index, best_distance);
    if (temp_2 >= best_distance) return;
    seek_nearest_with_kd(temp_direction > 0 ? n.right : n.left,
                         other_node_coordinates, index_dev, best_index, best_distance);
}

void KdTree::seek_nearest_linear(double * other_node_coordinates, int & best_index, double & best_distance)
{
    for (size_t i = 0; i < this->nodes.size(); i++) {
        KdNode & n = nodes[i]; // current node in the tree
        double temp_dist = get_distance(n.value, other_node_coordinates);
        if (temp_dist <= best_distance) {
            best_distance = temp_dist;
            best_index = i;
        }
    }
}

double * KdTree::get_node(int index)
{
    return nodes[index].value;
}

size_t KdTree::get_size()
{
    return nodes.size();
}
