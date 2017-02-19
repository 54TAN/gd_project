#include "Render.h"

#include <iostream>
#include <cmath>

void bresenham(bool ** temp_plain, GeomVector point_1, GeomVector point_2, bool edge_path, Bitmap * bmp)
{

    int a = point_2.y - point_1.y;
    int b = point_1.x - point_2.x;

    int sign = (std::abs(a) > std::abs(b)) ? 1 : -1;
    int sign_a = (a < 0) ? -1 : 1;
    int sign_b = (b < 0) ? -1 : 1;

    int f = 0;
    int x = (int)point_1.x;
    int y = (int)point_1.y; // текущие координаты
    int dy = a*sign_a;
    int dx = b*sign_b;
    int fin_x = point_2.x;
    int fin_y = point_2.y; // финальные координаты
    if (bmp != NULL) bmp->add_pix(x, y, 1, edge_path);
    else temp_plain[x][y] = true;
    bool op = true; // чтобы менять х и у при заполнении пикселями

    sign_b *= -1;
    if (sign == -1) {
        std::swap(x, y);
        std::swap(dx, dy);
        std::swap(sign_a, sign_b);
        std::swap(fin_x, fin_y);
        op = false;
    }

    do { // если > 45
        f += dx;
        if (f > 0) {
            f -= dy;
            x += sign_b;
        }
        y += sign_a;

        if (bmp != NULL) {
            if (op) bmp->add_pix(x, y, 1, edge_path); //додумать
            else  bmp->add_pix(y, x, 1, edge_path); //додумать

        } else {
            if (op) temp_plain[x][y] = true;
            else temp_plain[y][x] = true;
        }
    } while (x != fin_x || y != fin_y);
}

void render_map(Map the_map, Bitmap * bmp)
{
    for (int i = 0; i < bmp->width; i++) {
        for (int j = 0; j < bmp->height; j++) {
            GeomVector pt((double)i, (double)j);
            bool flag = !the_map.is_point_in_obstacle(pt) || the_map.among_points(pt);
            bmp->add_pix(i, j, flag, 0);
        }
    }

}

void render_path(std::vector<GeomVector> path, Bitmap * bmp, bool edge_path)
{/*
    bool ** temp_plain = new bool * [bmp->width];
    for (size_t i = 0; i < bmp->width; i++) {
        temp_plain[i] = new bool [bmp->height];
    }
    for (size_t i = 0; i < bmp->width; i++) {
        for (size_t j = 0; j < bmp->height; j++) {
            temp_plain[i][j] = false;
        }
    }
    //brezenhem(temp_plain, path[0], path[1]);*/
    bool ** some;
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (path[i].x == -1) {
            continue;
        }
        bresenham(some, path[i], path[i + 1], edge_path, bmp);
    }
    //
    /*
    for (size_t i = 0; i < bmp->width; i++) {
        for (size_t j = 0; j < bmp->height; j++) {
            if (temp_plain[i][j]) {
                GeomVector pt((double)i, (double)j);
                if (!the_map.is_point_in_obstacle(pt)) {
                    std::cout << "YOU SHELL NOT PAAAAASS!!!" << std::endl;
                    return;
                }
            }
        }
    }*/
    //std::cout << "/* message */" << std::endl;
    /*
    for (size_t i = 0; i < bmp->width; i++) {
        for (size_t j = 0; j < bmp->height; j++) {
            if (temp_plain[i][j]) {
                bmp->add_pix(i, j, true);
            }
        }
    }*/
}
