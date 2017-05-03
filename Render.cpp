#include "Render.h"
#include "Map.h"
#include "Bitmap.h"

#include <iostream>
#include <cmath>
#include <fstream>

void draw_line(Coordinates point_1, Coordinates point_2, Bitmap *bmp)
{
    double x1 = point_1.x;
    double y1 = point_1.y;
    double x2 = point_2.x;
    double y2 = point_2.y;

    const bool slope = (fabs(y2 - y1) > fabs(x2 - x1));

    if (slope) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const double dx = x2 - x1;
    const double dy = fabs(y2 - y1);

    double error = dx / 2.0;
    const int y_step = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int max_x = (int)x2;

    for(int x = (int)x1; x < max_x; x++) {
        if (slope) {
            bmp->add_pix(y, x, 1);
        } else {
            bmp->add_pix(x, y, 1);
        }
        error -= dy;
        if (error < 0) {
            y += y_step;
            error += dx;
        }
    }
}

void render_map(Map the_map, Bitmap *bmp)
{
    for (int i = 0; i < bmp->width; i++) {
        for (int j = 0; j < bmp->height; j++) {
            // true = black, false = white
            bool flag = the_map.is_point_in_obstacle(Coordinates((double) i, (double) j));
            bmp->add_pix(i, j, flag);
        }
    }
}

void render_path(std::vector<Coordinates> path, Bitmap *bmp) {
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (path[i].x == -1) {
            continue;
        }
        draw_line(path[i], path[i + 1], bmp);
    }
}
