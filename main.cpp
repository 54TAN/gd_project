#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include <iostream>
#include <cmath>
#include <ctime>

int main(int argc, char ** argv) {

    int width = 1000;
    int height = 500;

    srand(time(NULL));

    Map my_map;
    my_map.width = width;
    my_map.height = height;

    //my_map.obstacles.push_back(Obstacle(Point(180, 0), Point(190, 300)));
    my_map.obstacles.push_back(Obstacle(Point(380, 200), Point(390, 500)));
    my_map.obstacles.push_back(Obstacle(Point(780, 200), Point(790, 500)));
    //my_map.obstacles.push_back(Obstacle(Point(560, 330), Point(570, 500)));
    //my_map.obstacles.push_back(Obstacle(Point(490, 250), Point(700, 260)));
    //my_map.obstacles.push_back(Obstacle(Point(620, 430), Point(700, 440)));
    //my_map.obstacles.push_back(Obstacle(Point(660, 460), Point(670, 500))); //финальная висяка*//*
    my_map.obstacles.push_back(Obstacle(Point(580, 0), Point(590, 300)));
    my_map.points.push_back(Point(2, 498));
    my_map.points.push_back(Point(998, 498));

    clock_t t = clock();

    RrTree my_rrt(&my_map, 50, 1); // все финальные тесты на 50
    my_rrt.get_path(my_rrt.nodes.size() - 1);

    t = (clock() - t) / CLOCKS_PER_SEC;

    int hours = t / 3600;
    int minutes = (t - hours*3600) / 60;
    int seconds = t - hours*3600 - minutes*60;

    std::cout << "points map : " << my_map.points.size() << '\n';
    std::cout << "rrt points : " << my_rrt.nodes.size() << '\n';
    std::cout << "path points : " << my_rrt.path.size() << '\n';
    std::cout << "time : " << hours << ":" << minutes << ":" << seconds << std::endl;

    Bitmap my_bmp(width, height);
    render_map(my_map, &my_bmp);
    for (size_t i = 0; i < my_rrt.nodes.size(); i++) {
        my_rrt.go(i);
        if (my_rrt.edges.size())
            render_path(my_rrt.edges, &my_bmp, 0);
    }
    render_path(my_rrt.path, &my_bmp, 1);
    my_bmp.out_bmp("MAP_PATH.bmp");

    return 0;
}