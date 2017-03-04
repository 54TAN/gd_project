#include "Render.h"

#include <iostream>
#include <cmath>
#include <fstream>

void bresenham(bool ** temp_plain, Coordinates point_1, Coordinates point_2,
               bool edge_path, Bitmap * bmp, bool optimized, bool for_moving,
               std::vector <Coordinates>* coords)
{

    int a = point_2.y - point_1.y;
    int b = point_1.x - point_2.x;

    int sign = (std::abs(a) > std::abs(b)) ? 1 : -1;
    int sign_a = (a < 0) ? -1 : 1;
    int sign_b = (b < 0) ? -1 : 1;

    int f = 0;
    int x = (int)point_1.x;
    int y = (int)point_1.y; // текущие координаты
    //std::cout << y << std::endl;
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
            if (!for_moving) {
                if (op) bmp->add_pix(x, y, 1, edge_path, optimized); //додумать
                else bmp->add_pix(y, x, 1, edge_path, optimized); //додумать
            } else {
                if (op) coords->push_back(Coordinates(x, y));
                else coords->push_back(Coordinates(y, x));
            }
        } else {
            //std::cout << y << " ";
            if (op) temp_plain[x][y] = true;
            else temp_plain[y][x] = true;
        }
    } while (x != fin_x || y != fin_y);
    std::cout << std::endl;
}

void render_map(Map the_map, Bitmap * bmp)
{
    for (int i = 0; i < bmp->width; i++) {
        for (int j = 0; j < bmp->height; j++) {
            Coordinates pt((double)i, (double)j);
            bool flag = the_map.is_point_in_obstacle(pt) || the_map.among_points(pt);
            bmp->add_pix(i, j, flag, 0);
        }
    }

}

void render_path(std::vector<Coordinates> path, Bitmap * bmp, bool edge_path, bool optimized,
                 bool for_moving,
                 std::vector <Coordinates>* coords)
{
    bool ** some;
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (path[i].x == -1) {
            continue;
        }
        bresenham(some, path[i], path[i + 1], edge_path, bmp, optimized, for_moving, coords);
    }
}

void bresenham_obj(bool **temp_plain, Coordinates const& object_1, Coordinates const& object_2) {

    double end_object_1_x = object_1.x + object_1.length * cos(object_1.phi * M_PI / 180);
    double end_object_1_y = object_1.y + object_1.length * sin(object_1.phi * M_PI / 180);
    Coordinates end_1(end_object_1_x, end_object_1_y);

    double end_object_2_x = object_2.x + object_2.length * cos(object_2.phi * M_PI / 180);
    double end_object_2_y = object_2.y + object_2.length * sin(object_2.phi * M_PI / 180);
    Coordinates end_2(end_object_2_x, end_object_2_y);

    bresenham(temp_plain, object_1, end_1, 0);
    bresenham(temp_plain, object_2, end_2, 0);
    bresenham(temp_plain, end_1, end_2, 0);
    bresenham(temp_plain, object_1, object_2, 0);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; ++j) {
            if (temp_plain[i][j] == true) {
                for (int k = j; k <= object_1.length + j; k++)
                    temp_plain[i][k] = true;
                break;
            }
        }
    }

    {
        std::ofstream file("temp_plain.txt");
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                file << temp_plain[i][j] << " ";
            }
            file << "\n";
        }
    }


}
