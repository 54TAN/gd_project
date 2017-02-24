#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <cmath>
#include <ctime>

void make_map(int width, int height, Map & map) {
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Point(380, 200), Point(390, 500)));
    map.obstacles.push_back(Obstacle(Point(780, 200), Point(790, 500)));
    map.obstacles.push_back(Obstacle(Point(580, 0), Point(590, 300)));
    map.points.push_back(Point(2, 498));
    map.points.push_back(Point(998, 498));
}

void out_info(RrTree const& rrt, Map const& map, clock_t time) {
    int hours = time / 3600;
    int minutes = (time - hours*3600) / 60;
    int seconds = time - hours*3600 - minutes*60;
    std::cout << "points map : " << map.points.size() << '\n';
    std::cout << "rrt points : " << rrt.nodes.size() << '\n';
    std::cout << "path points : " << rrt.path.size() << '\n';
    std::cout << "time : " << hours << ":" << minutes << ":" << seconds << std::endl;
}

void make_picture(RrTree & rrt, Map const& map) {
    Bitmap my_bmp(map.width, map.height);
    render_map(map, &my_bmp);
    for (size_t i = 0; i < rrt.nodes.size(); i++) {
        rrt.go(i);
        if (rrt.edges.size())
            render_path(rrt.edges, &my_bmp, 0);
    }
    render_path(rrt.path, &my_bmp, 1);
    my_bmp.out_bmp("MAP_PATH.bmp");
}

int main(int argc, char ** argv) {

    srand(time(NULL));

    int width = 1000;
    int height = 500;

    Map my_map;
    make_map(width, height, my_map);

    clock_t t = clock();

    RrTree my_rrt(&my_map, 50, 1);
    my_rrt.get_path(my_rrt.nodes.size() - 1);

    t = (clock() - t) / CLOCKS_PER_SEC;

    out_info(my_rrt, my_map, t);
    make_picture(my_rrt, my_map);

    return 0;
}