#include "Render.h"

#include <iostream>
#include <cmath>
#include <fstream>

void bresenham(bool ** temp_plane, Coordinates point_1, Coordinates point_2,
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
    else temp_plane[x][y] = true;
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
            if (op) temp_plane[x][y] = true;
            else temp_plane[y][x] = true;
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

void bresenham_circle(bool ** temp_plane, Coordinates point,
                      int bound_x_up, int bound_y_up,
                      int bound_x_down, int bound_y_down) {

    int x = point.x;
    int y = point.y;


    int x1, y1, yk = 0;
    int sigma, delta, f;

    x1 = 0;
    y1 = point.length;
    delta = 2 * (1 - point.length);


    do {
        if (bound_x_down <= (x + x1) && (x + x1) <= bound_x_up &&
            bound_y_down <= (y + y1) && (y + y1) <= bound_y_up) {
            temp_plane[x + x1][y + y1] = true;
        }
        if (bound_x_down <= (x - x1) && (x - x1) <= bound_x_up &&
            bound_y_down <= (y + y1) && (y + y1) <= bound_y_up) {
            temp_plane[x - x1][y + y1] = true;
        }
        if (bound_x_down <= (x + x1) && (x + x1) <= bound_x_up &&
            bound_y_down <= (y - y1) && (y - y1) <= bound_y_up) {
            temp_plane[x + x1][y - y1] = true;
        }
        if (bound_x_down <= (x - x1) && (x - x1) <= bound_x_up &&
            bound_y_down <= (y - y1) && (y - y1) <= bound_y_up) {
            temp_plane[x - x1][y - y1] = true;
        }


        f = 0;

        if (y1 < yk) break;

        if (delta < 0) {

            sigma = 2 * (delta + y1) - 1;

            if (sigma <= 0) {
                x1++;
                delta += 2 * x1 + 1;
                f = 1;
            }

        } else if (delta > 0) {

            sigma = 2 * (delta - x1) - 1;

            if (sigma > 0) {
                y1--;
                delta += 1 - 2 * y1;
                f = 1;
            }
        }

        if (!f) {
            x1++;
            y1--;
            delta += 2 * (x1 - y1 - 1);
        }

    } while (true);
}

void bresenham_obj(bool **temp_plane, Coordinates object_1, Coordinates object_2) {

    int end_object_1_x = object_1.x + object_1.length * cos(object_1.phi * M_PI / 180);
    int end_object_1_y = object_1.y + object_1.length * sin(object_1.phi * M_PI / 180);
    Coordinates end_1(end_object_1_x, end_object_1_y);

    int end_object_2_x = object_2.x + object_2.length * cos(object_2.phi * M_PI / 180);
    int end_object_2_y = object_2.y + object_2.length * sin(object_2.phi * M_PI / 180);
    Coordinates end_2(end_object_2_x, end_object_2_y);

    bresenham(temp_plane, object_1, end_1, 0);

    bresenham(temp_plane, object_2, end_2, 0);

    bresenham(temp_plane, object_1, object_2, 0);

    bresenham(temp_plane, end_1, Coordinates(object_2.x,
                                             end_object_1_y + object_2.y - object_1.y), 0);


    int initial_x = std::min(object_1.x, object_2.x);
    //std::cout << initial_x << "\n";
    int final_x = std::max(object_1.x, object_2.x);
    //std::cout << final_x << "\n";
    int initial_y = std::min(object_1.x, object_2.x);
    int final_y = std::min(object_1.x, object_2.x);

    Coordinates center(object_2.x, object_2.y, 0, object_1.length);

    int bound_x_up = std::max(end_object_2_x, (int)object_1.x);
    int bound_y_up = std::max(end_object_1_y + (int)object_2.y, (int)object_2.y);
    int bound_x_down = std::min((int)object_1.x, (int)object_2.x);
    int bound_y_down = std::min((int)object_1.y, (int)object_2.y);

    bresenham_circle(temp_plane, center, bound_x_up, bound_y_up, bound_x_down, bound_y_down);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; ++j) {

            if (i >= final_x && i <= initial_x) {
                temp_plane[i][j] = false;
            }

            if (temp_plane[i][j]) {
                int tmp = j + 1;
                while (!temp_plane[i][tmp] && tmp < height) {
                    temp_plane[i][tmp] = true;
                    tmp++;
                }

                for (int k = tmp + 1; k < height; k++) {
                    temp_plane[i][k] = false;
                }

                break;
            }
        }
    }


    {
        std::ofstream file("temp_plane.txt");
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                file << temp_plane[i][j] << " ";
            }
            file << "\n";
        }
    }


}
