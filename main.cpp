#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"
#include "Feet.h"

#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>

static Bitmap bmp(width, height);
static Map map;

void make_map() 
{
    map.height = height;
    map.width = width;
    
    map.obstacles.push_back(Obstacle(Coordinates(295, 0), Coordinates(305, 400)));
    map.obstacles.push_back(Obstacle(Coordinates(695, 200), Coordinates(705, 600)));

    map.points.emplace_back(Coordinates(80, 80), 90, 80, 100);
    //map.points.emplace_back(Coordinates(100, 400), 60, 80, 100);
    map.points.emplace_back(Coordinates(870, 450), 90, 80, 100);

    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
}

int main(int argc, char ** argv) 
{
    srand(time(NULL));

    make_map();

    RrTree rrt(&map, 1000);
    rrt.search(&map, 1, &bmp);
    rrt.get_path(rrt.nodes.size() - 1);
    std::reverse(rrt.path.begin(), rrt.path.end());
    std::cout << "the path has gotten\n";
    rrt.optimize_path(&map);
    std::cout << "the path has optimized\n";

    Feet start(rrt.path.front(), 1);
    render_feet(start, &bmp, 0);
    for (auto i = 1; i < rrt.path.size() - 2; i++) {
        Feet feet;
        if (rrt.path[i].left_to_up.x == rrt.path[i + 1].left_to_up.x &&
            rrt.path[i].left_to_up.y == rrt.path[i + 1].left_to_up.y)
        {
            feet = Feet(rrt.path[i], 1);
        } else {
            feet =  Feet(rrt.path[i], 0);
        }
        render_feet(feet, &bmp, 0);
        render_contour(rrt.path[i], &bmp, 0);
    }
    Feet finish(rrt.path.back(), 1);
    render_feet(finish, &bmp, 0);

    bmp.out_bmp("MAP_PATH.bmp"); 
    return 0;
}
