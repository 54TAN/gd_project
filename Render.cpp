#include "Render.h"

#include <iostream>
#include <cmath>

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
    int dy = a*sign_a;
    int dx = b*sign_b;
    int fin_x = point_2.x;
    int fin_y = point_2.y; // финальные координаты
    if (fin_x >= width || fin_y >= height || fin_x < 0 || fin_y < 0)
        return;
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
            if (op && x < width && y < height){ temp_plain[x][y] = true; }
            else {
                if (y < width && x < height)
                    temp_plain[y][x] = true;
            }
        }
    } while (x != fin_x || y != fin_y);
}

void render_map(Map the_map, Bitmap * bmp)
{
    for (int i = 0; i < bmp->width; i++) {
        for (int j = 0; j < bmp->height; j++) {
            Coordinates pt((double)i, (double)j);
            bool flag = !the_map.is_point_in_obstacle(pt) || the_map.among_points(pt);
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

void bresenham_obj(bool **temp_plain, Coordinates point_1, Coordinates point_2) {

    int a = point_2.y - point_1.y;
    int b = point_1.x - point_2.x;

    int sign = (std::abs(a) > std::abs(b)) ? 1 : -1;
    int sign_a = (a < 0) ? -1 : 1;
    int sign_b = (b < 0) ? -1 : 1;

    int f = 0;
    int x = (int)point_1.x;
    int y = (int)point_1.y; // текущие координаты

    //тут вызывается брезенхем для линии, и заполняет всю мою палку
    double end_x = point_1.x + point_1.length * cos(point_1.phi * M_PI / 180);
    double end_y = point_1.y + point_1.length * sin(point_1.phi * M_PI / 180);
    Coordinates end(end_x, end_y);
    bresenham(temp_plain, point_1, end, 0);
    //

    int dy = a*sign_a;
    int dx = b*sign_b;
    int fin_x = point_2.x;
    int fin_y = point_2.y; // финальные координаты
    temp_plain[x][y] = true;
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

        if (op) {
            //std::cout << "Sdsdsdsd" << std::endl;
            temp_plain[x][y] = true;
            double end_x = point_1.x + point_1.length * cos(point_1.phi * M_PI / 180);
            double end_y = point_1.y + point_1.length * sin(point_1.phi * M_PI / 180);
            Coordinates end(end_x, end_y);
            bresenham(temp_plain, Coordinates(x, y), end, 0);
        } else {
            temp_plain[y][x] = true;
            double end_x = point_1.x + point_1.length * cos(point_1.phi * M_PI / 180);
            double end_y = point_1.y + point_1.length * sin(point_1.phi * M_PI / 180);
            Coordinates end(end_y, end_x);
            bresenham(temp_plain, Coordinates(y, x), end, 0);
        }

    } while (x != fin_x || y != fin_y);
}
