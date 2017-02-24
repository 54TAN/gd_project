#ifndef GD_PROJECT_RENDER_H
#define GD_PROJECT_RENDER_H


#include <vector>
#include <clocale>
#include "Bitmap.h"
#include "Map.h"

void bresenham(bool ** temp_plain, Point point_1, Point point_2, bool edge_path,
               Bitmap * bmp = NULL, bool optimized = 0);
void render_map(Map the_map, Bitmap * bmp);
void render_path(std::vector<Point> path, Bitmap * bmp, bool edge_path, bool optimized = 0);


#endif //GD_PROJECT_RENDER_H
