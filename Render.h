#ifndef GD_PROJECT_RENDER_H
#define GD_PROJECT_RENDER_H


#include <vector>
#include <clocale>

#include "Consts.h"
#include "Bitmap.h"
#include "Map.h"

void render_map(Map, Bitmap*);
void render_path(std::vector<Coordinates> path, Bitmap * bmp, bool edge_path, bool optimized = 0,
                 bool for_moving = false,
                 std::vector <Coordinates> * coords = NULL);
void draw_line(Coordinates point_1, Coordinates point_2,
          bool edge_path, Bitmap *bmp, bool optimized);

#endif //GD_PROJECT_RENDER_H
