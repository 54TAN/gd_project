#include "Render.h"
#include "Map.h"
#include "Bitmap.h"

#include <iostream>
#include <cmath>
#include <fstream>
/*
void bresenham(bool **temp_plane, Coordinates point_1, Coordinates point_2,
               bool edge_path, Bitmap *bmp, bool optimized, bool for_moving,
               std::vector<Coordinates> *coords) {

    if ((int)point_1.x == (int)point_2.x) {
        //std::cout << point_1.x << " pointttttttttttttttttttt\n";
        int x = point_1.x;
        for (int y = std::min(point_1.y, point_2.y); 
                 y < std::max(point_1.y, point_2.y); y++) {
            if (bmp != NULL) {
                if (!for_moving) {
                    bmp->add_pix(x, y, 1, edge_path, optimized); //додумать
                } else {
                    coords->push_back(Coordinates(x, y));
                }
            } else {
                if (coords != NULL) {
                    coords->push_back(Coordinates(x, y));
                    continue;
                }
                temp_plane[x][y] = true;
            }
    
        }
        return;
    }
    
    if ((int)point_1.y == (int)point_2.y) {
        int y = point_1.y;
        for (int x = std::min(point_1.x, point_2.x); 
                 x < std::max(point_1.x, point_2.x); x++) {
            if (bmp != NULL) {
                if (!for_moving) {
                    bmp->add_pix(x, y, 1, edge_path, optimized); //додумать
                } else {
                    coords->push_back(Coordinates(x, y));
                }
            } else {
                if (coords != NULL) {
                    coords->push_back(Coordinates(x, y));
                    continue;
                }
                temp_plane[x][y] = true;
            }
    
        }
        return;
    }



    int a = point_2.y - point_1.y;
    int b = point_1.x - point_2.x;

    int sign = (std::abs(a) > std::abs(b)) ? 1 : -1;
    int sign_a = (a < 0) ? -1 : 1;
    int sign_b = (b < 0) ? -1 : 1;

    int f = 0;
    int x = (int)point_1.x;
    int y = (int)point_1.y; // текущие координаты
    //std::cout << y << std::endl;
    int dy = a * sign_a;
    int dx = b * sign_b;
    int fin_x = (int)point_2.x;
    int fin_y = (int)point_2.y; // финальные координаты
    if (bmp != NULL) bmp->add_pix(x, y, 1, edge_path);
    //else //temp_plane[x][y] = true;
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
            //std::cout << x << " " << y << " //// " << fin_x << " " << fin_y << "\n";
            //if (x == fin_x || fin_y == y) break;
            if (!for_moving) {
                if (op) bmp->add_pix(x, y, 1, edge_path, optimized); //додумать
                else bmp->add_pix(y, x, 1, edge_path, optimized); //додумать
            } else {
                if (op) coords->push_back(Coordinates(x, y));
                else coords->push_back(Coordinates(y, x));
            }
        } else {
            //std::cout << y << " ";
            //std::cout << x << std::endl;
            if (coords != NULL) {
                //std::cout << "in\n";
                if (op) coords->push_back(Coordinates(x, y));
                else coords->push_back(Coordinates(y, x));
                continue;
            }
            if (op) temp_plane[x][y] = true;
            else temp_plane[y][x] = true;
        }
    } while (x != fin_x && y != fin_y);
//    std::cout << std::endl;

}
*/
void draw_line(Coordinates point_1, Coordinates point_2, Bitmap *bmp, bool optimized)
{
    double x1 = point_1.x;
    double y1 = point_1.y;
    double x2 = point_2.x;
    double y2 = point_2.y;

    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));

    if (steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++) {
        if (steep) {
            bmp->add_pix(y, x, 1, 0, optimized);
        } else {
            bmp->add_pix(x, y, 1, 0, optimized);
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void render_map(Map the_map, Bitmap *bmp) 
{
    for (int i = 0; i < bmp->width; i++) {
        for (int j = 0; j < bmp->height; j++) {
            Coordinates pt((double) i, (double) j);
            bool flag = the_map.is_point_in_obstacle(pt);// || the_map.among_points(pt);
            bmp->add_pix(i, j, flag, 0);
        }
    }
}

void render_path(std::vector<Coordinates> path, Bitmap *bmp, bool edge_path, bool optimized,
                 bool for_moving,
                 std::vector<Coordinates> * coords) {
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (path[i].x == -1) {
            continue;
        }
        draw_line(path[i], path[i + 1], bmp, optimized);
    }
}
