#ifndef GD_PROJECT_RENDER_H
#define GD_PROJECT_RENDER_H


#include <vector>
#include <clocale>

#include "Consts.h"
#include "Bitmap.h"
#include "Map.h"

void bresenham(bool ** temp_plain, Coordinates point_1, Coordinates point_2, bool edge_path,
               Bitmap * bmp = NULL, bool optimized = 0, bool for_moving = false,
               std::vector <Coordinates>* coords = NULL);

void bresenham_obj(bool ** temp_plain, Coordinates const& point_1, Coordinates const& point_2);
void render_map(Map the_map, Bitmap * bmp);
void render_path(std::vector<Coordinates> path, Bitmap * bmp, bool edge_path, bool optimized = 0,
                 bool for_moving = false,
                 std::vector <Coordinates>* coords = NULL);


#endif //GD_PROJECT_RENDER_H
