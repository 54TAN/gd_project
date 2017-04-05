#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include <iostream>
#include <algorithm>
#include <ctime>

static Bitmap bmp(width, height);
static Map map;

void make_map() 
{
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Coordinates(95, 0), Coordinates(105, 100)));

    map.points.push_back(Coordinates(2, 2, 90, 60));
    map.points.push_back(Coordinates(198, 2, 90, 60));

    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
}

int main(int argc, char ** argv) 
{
    srand(time(NULL));

    make_map();

    RrTree rrt(&map, 500);
    rrt.search(&map, 1);
    rrt.get_path(rrt.nodes.size() - 1);
	rrt.optimize_path(&map);

    Coordinates MovableObject(2, 2, 90, 60);

    for (auto item : rrt.path) {
		MovableObject.x = item.x;
		MovableObject.y = item.y;
       	std::vector <Coordinates> current_coords;
		int end_MovableObject_x = (MovableObject.x + MovableObject.length * cos(item.phi * M_PI / 180));
        int end_MovableObject_y = MovableObject.y + MovableObject.length * sin(item.phi * M_PI / 180);

        current_coords.push_back(MovableObject);
        current_coords.push_back(Coordinates(end_MovableObject_x, end_MovableObject_y));

        render_path(current_coords, &bmp, 0);
    }  

	bmp.out_bmp("MAP_PATH.bmp"); 

    return 0;
}
