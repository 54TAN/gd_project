#ifndef GD_PROJECT_RENDER_H
#define GD_PROJECT_RENDER_H


#include <vector>
#include <clocale>

#include "Consts.h"
#include "Bitmap.h"
#include "Map.h"

void render_map(Map, Bitmap*);
void render_path(std::vector<Coordinates> path, Bitmap * bmp);
void draw_line(Coordinates point_1, Coordinates point_2, Bitmap *bmp);

#endif //GD_PROJECT_RENDER_H
