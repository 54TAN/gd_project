#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include <iostream>
#include <cmath>
#include <ctime>

int main(int argc, char ** argv) {

    int width = 700;
    int height = 500;

    srand(time(NULL));

    Map my_map;
    my_map.width = width;
    my_map.height = height;

    my_map.obstacles.push_back(Obstacle(GeomVector(180, 0), GeomVector(190, 300)));
    my_map.obstacles.push_back(Obstacle(GeomVector(380, 120), GeomVector(390, 500)));
    /*my_map.obstacles.push_back(Obstacle(GeomVector(560, 330), GeomVector(570, 500)));
    my_map.obstacles.push_back(Obstacle(GeomVector(490, 250), GeomVector(700, 260)));
    my_map.obstacles.push_back(Obstacle(GeomVector(620, 430), GeomVector(700, 440)));
    my_map.obstacles.push_back(Obstacle(GeomVector(660, 460), GeomVector(670, 500))); //финальная висяка*/
    my_map.points.push_back(GeomVector(2, 2));
    my_map.points.push_back(GeomVector(698, 498));

    RrTree my_rrt(&my_map, 50, 1); // все финальные тесты на 50
    std::cout << "points map: " << my_map.points.size() << '\n';
    my_rrt.get_path(my_rrt.nodes.size() - 1);
    std::cout << "rrt points : "<< my_rrt.nodes.size() << '\n';
    std::cout << "path points : "<< my_rrt.path.size() << '\n';

    Bitmap my_bmp(width, height);
    render_map(my_map, &my_bmp);
    for (size_t i = 0; i < my_rrt.nodes.size(); i++) {
        my_rrt.go(i);
        if (my_rrt.edges.size())
            render_path(my_rrt.edges, &my_bmp, 0);
    }
    render_path(my_rrt.path, &my_bmp, 1);
    my_bmp.out_bmp("MAP_PATH.bmp");

    /*bitmap_t my_bmp(width, height);
    render_map(my_map, &my_bmp);
    for (size_t i = 0; i < my_rrt.nodes.size(); i++) {
        my_rrt.go(i);
        if (my_rrt.edges.size())
            render_path(my_rrt.edges, &my_bmp, 0);
    }
    render_path(my_rrt.path, &my_bmp, 1);
    my_bmp.out_bmp("MAP_PATH.bmp");*/

    return 0;
}