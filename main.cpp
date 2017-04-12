#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>

static std::vector <Coordinates> go_path;
static std::vector <Coordinates> replicated_path;
static Coordinates MovableObject(2, 2, 90, 60);
static Bitmap bmp(width, height);
static Map map;

void make_map() 
{
    map.height = height;
    map.width = width;
    
    map.obstacles.push_back(Obstacle(Coordinates(295, 0), Coordinates(305, 400)));
    map.obstacles.push_back(Obstacle(Coordinates(695, 200), Coordinates(705, 600)));

    map.points.push_back(Coordinates(2, 2, 90, 60));
    map.points.push_back(Coordinates(998, 530, 90, 60));

    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
}
int main(int argc, char ** argv) 
{
    srand(time(NULL));

    make_map();

    RrTree rrt(&map, 300);
    rrt.search(&map, 1);
    rrt.get_path(rrt.nodes.size() - 1);
    std::cout << "before " << rrt.path.size() << "\n";
    rrt.optimize_path(&map, 2, 4);
    std::cout << "after " << rrt.path.size() << "\n";
	
    Coordinates Movableobject(2, 2, 90, 60);
    Coordinates Previous(Movableobject);

    for (auto item : rrt.path) {
        Previous = MovableObject;

		MovableObject.x = item.x;
		MovableObject.y = item.y;
        
       	std::vector <Coordinates> current_coords;
        std::vector <Coordinates> heels;

		int end_MovableObject_x = (MovableObject.x + MovableObject.length * cos(item.phi * M_PI / 180));
        int end_MovableObject_y = MovableObject.y + MovableObject.length * sin(item.phi * M_PI / 180);

        current_coords.push_back(MovableObject);
        current_coords.push_back(Coordinates(end_MovableObject_x, end_MovableObject_y));

        heels.push_back(MovableObject);
        heels.push_back(Previous);

        render_path(current_coords, &bmp, 0);
        if (item != rrt.path.front())render_path(heels, &bmp, 1);
    }  

    bmp.out_bmp("MAP_PATH.bmp"); 
    return 0;
}
