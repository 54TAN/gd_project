#include "Render.h"

#include <iostream>
#include <cmath>
#include <fstream>

void bresenham(bool **temp_plane, Coordinates point_1, Coordinates point_2,
               bool edge_path, Bitmap *bmp, bool optimized, bool for_moving,
               std::vector<Coordinates> *coords) {

    int a = point_2.y - point_1.y;
    int b = point_1.x - point_2.x;

    int sign = (std::abs(a) > std::abs(b)) ? 1 : -1;
    int sign_a = (a < 0) ? -1 : 1;
    int sign_b = (b < 0) ? -1 : 1;

    int f = 0;
    int x = (int) point_1.x;
    int y = (int) point_1.y; // текущие координаты
    //std::cout << y << std::endl;
    int dy = a * sign_a;
    int dx = b * sign_b;
    int fin_x = point_2.x;
    int fin_y = point_2.y; // финальные координаты
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
    } while (x != fin_x || y != fin_y);
//    std::cout << std::endl;
}

void render_map(Map the_map, Bitmap *bmp) {
    for (int i = 0; i < bmp->width; i++) {
        for (int j = 0; j < bmp->height; j++) {
            Coordinates pt((double) i, (double) j);
            bool flag = the_map.is_point_in_obstacle(pt) || the_map.among_points(pt);
            bmp->add_pix(i, j, flag, 0);
        }
    }

}

void render_path(std::vector<Coordinates> path, Bitmap *bmp, bool edge_path, bool optimized,
                 bool for_moving,
                 std::vector<Coordinates> * coords) {
    bool **some;
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (path[i].x == -1) {
            continue;
        }
        bresenham(some, path[i], path[i + 1], edge_path, bmp, optimized, for_moving, coords);
    }
}

void bresenham_circle(bool **temp_plane, Coordinates point,
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

    if (object_1.x != object_2.x || object_1.y != object_2.y) {
        Coordinates left = (object_1.x > object_2.x) ? object_2 : object_1;
        Coordinates right = (object_1.x < object_2.x) ? object_2 : object_1;
        Coordinates up = (object_1.y > object_2.y) ? object_2 : object_1;
        Coordinates down = (object_1.y < object_2.y) ? object_2 : object_1;

        int end_left_x = (left.x + left.length * cos(left.phi * M_PI / 180));
        int end_left_y = left.y + left.length * sin(left.phi * M_PI / 180);
        Coordinates end_left(end_left_x, end_left_y);

        //int phi = (right.phi > 90) ? 180 - right.phi : right.phi;

        int end_right_x = right.x + right.length * cos(right.phi * M_PI / 180);
        int end_right_y = right.y + right.length * sin(right.phi * M_PI / 180);
        Coordinates end_right(end_right_x, end_right_y);

        bresenham(temp_plane, left, end_left, 0);
        bresenham(temp_plane, right, end_right, 0);
        bresenham(temp_plane, left, right, 0);

        if (left.phi != right.phi && (left.x != right.x && right.y != left.y)) {

            if (right.phi < left.phi && end_left_x < end_right_x) {
                int diff_x = (left.phi < 90) ? abs(end_left_x - left.x) : -1 * abs(end_left_x - left.x);
                std::cout << "one";
                if (right.y < left.y) {
                    bresenham(temp_plane, end_left, Coordinates(right.x + right.length * cos(left.phi * M_PI / 180),
                                                                end_left_y - (left.y - right.y)), 0);
                } else {
                    bresenham(temp_plane, end_left, Coordinates(right.x + diff_x, end_left_y + abs(right.y - left.y)),
                              0);
                }
                Coordinates center(right.x, right.y, 0, object_1.length);

                int bound_x_up = end_right_x - 1;
                int bound_y_up = height;
                int bound_x_down = right.x + diff_x;
                int bound_y_down = std::min((int) left.y, (int) right.y);

                bresenham_circle(temp_plane, center, bound_x_up, bound_y_up, bound_x_down, bound_y_down);
            }

            /*if (right.phi < 90 && right.phi < left.phi && end_left_x > right.x)
                bresenham(temp_plane, end_left, Coordinates(right.x + right.length * cos(left.phi * M_PI / 180),
                                                            right.y + right.length * sin(left.phi * M_PI / 180)), 0);*/

            if (right.phi > left.phi && end_left_x > end_right_x) {
                bresenham(temp_plane, right, Coordinates(right.x + right.length * cos(left.phi * M_PI / 180),
                                                         right.y + right.length * sin(left.phi * M_PI / 180)), 0);
                std::cout << "two";
                if (right.y < left.y) {
                    bresenham(temp_plane, end_left, Coordinates(right.x + right.length * cos(left.phi * M_PI / 180),
                                                                end_left_y - (left.y - right.y)), 0);
                }

                Coordinates center(right.x, right.y, 0, object_1.length);

                int bound_x_up = right.x + right.length * cos(left.phi * M_PI / 180) - 1;
                int bound_y_up = std::max((int) (right.y + right.length * sin(left.phi * M_PI / 180)), end_right_y) + 1;
                int bound_x_down = end_right_x;
                int bound_y_down = std::min(end_right_y, (int) (right.y + right.length * sin(left.phi * M_PI / 180)));

                bresenham_circle(temp_plane, center, bound_x_up, bound_y_up, bound_x_down, bound_y_down);

            }

            /*if (right.phi < left.phi && left.phi <= 90) {
                std::cout << "three";

                if (right.y < left.y) {
                    bresenham(temp_plane, end_left, Coordinates(right.x + right.length * cos(left.phi * M_PI / 180),
                                                                end_left_y - (left.y - right.y)), 0);
                }

                Coordinates center(right.x, right.y, 0, object_1.length);

                int bound_x_up = end_right_x - 1;
                int bound_y_up = (int)(right.y + right.length * sin(left.phi * M_PI / 180)) + 1;
                int bound_x_down = end_left_x;
                int bound_y_down = std::min((int)left.y, (int)right.y);

                bresenham_circle(temp_plane, center, bound_x_up, bound_y_up, bound_x_down, bound_y_down);
            }*/

        } else {
            if (left.x != right.x && right.y != left.y)
                bresenham(temp_plane, end_left, end_right, 0);

        }
    } else {

        if (object_1.phi != object_2.phi) {
            if (object_1.x == object_2.x && object_1.y == object_2.y) {
                Coordinates left = (object_1.phi < object_2.phi) ? object_2 : object_1;
                Coordinates right = (object_1.phi > object_2.phi) ? object_2 : object_1;
                //std::cout << (left.phi == right.phi);
                int end_left_x = (left.x + left.length * cos(left.phi * M_PI / 180));
                int end_left_y = left.y + left.length * sin(left.phi * M_PI / 180);
                int end_right_x = right.x + right.length * cos(right.phi * M_PI / 180);
                int end_right_y = right.y + right.length * sin(right.phi * M_PI / 180);

                Coordinates end_left(end_left_x, end_left_y);
                Coordinates end_right(end_right_x, end_right_y);

                bresenham(temp_plane, left, end_left, 0);
                bresenham(temp_plane, right, end_right, 0);
                bresenham_circle(temp_plane, object_1, width, height, 0, 0);
            } else {
                std::cout << "here\n";

            }

        } else {

            Coordinates left = (object_1.x > object_2.x) ? object_2 : object_1;
            Coordinates right = (object_1.x < object_2.x) ? object_2 : object_1;
            Coordinates up = (object_1.y > object_2.y) ? object_2 : object_1;
            Coordinates down = (object_1.y < object_2.y) ? object_2 : object_1;

            int end_left_x = (left.x + left.length * cos(left.phi * M_PI / 180));
            int end_left_y = left.y + left.length * sin(left.phi * M_PI / 180);
            Coordinates end_left(end_left_x, end_left_y);

            //int phi = (right.phi > 90) ? 180 - right.phi : right.phi;

            int end_right_x = right.x + right.length * cos(right.phi * M_PI / 180);
            int end_right_y = right.y + right.length * sin(right.phi * M_PI / 180);
            Coordinates end_right(end_right_x, end_right_y);

            bresenham(temp_plane, left, end_left, 0);
            bresenham(temp_plane, right, end_right, 0);
            bresenham(temp_plane, left, right, 0);
            bresenham(temp_plane, end_left, end_right, 0);

        }
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; ++j) {
            if (!temp_plane[i][j]) {
                bool one_side = false;
                bool other_side = false;
                for (int k = 0; k < j; k++) {
                    if (temp_plane[i][k]) {
                        one_side = true;
                    }
                }
                for (int k = j + 1; k < height; k++) {
                    if (temp_plane[i][k]) {
                        other_side = true;
                    }
                }
                if (one_side && other_side)
                    temp_plane[i][j] = true;
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
