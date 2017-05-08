#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"
#include "Feet.h"

#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>

static Bitmap bmp(width, height);
static Map map;

void make_map() 
{
    map.height = height;
    map.width = width;
    
    map.obstacles.push_back(Obstacle(Coordinates(295, 0), Coordinates(305, 400)));
    map.obstacles.push_back(Obstacle(Coordinates(695, 200), Coordinates(705, 600)));

    map.points.emplace_back(Coordinates(80, 80), 90, 80, 100);
    map.points.emplace_back(Coordinates(90, 450), 90, 80, 100);
    //map.points.emplace_back(Coordinates(870, 450), 90, 80, 100);

    //obstacles for pont only
    /*map.obstacles.push_back(Obstacle(Coordinates(0, 240), Coordinates(160, 250)));
    map.obstacles.push_back(Obstacle(Coordinates(840, 350), Coordinates(1000, 360)));
    map.obstacles.push_back(Obstacle(Coordinates(295, 550), Coordinates(690, 560)));
    map.obstacles.push_back(Obstacle(Coordinates(310, 40), Coordinates(705, 50)));
    map.obstacles.push_back(Obstacle(Coordinates(495, 200), Coordinates(505, 400)));
    //obstacles for pont only*/

    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
}

int main(int argc, char ** argv) 
{
    srand(time(NULL));

    make_map();

    RrTree rrt(&map, 200);
    rrt.search(&map, 1, &bmp);
    rrt.get_path(rrt.nodes.size() - 1);
    std::reverse(rrt.path.begin(), rrt.path.end());
    std::cout << "the path has gotten\n";
    rrt.optimize_path(&map);
    std::cout << "the path has optimized\n";

    std::ofstream config("feet.config");

    Feet start(rrt.path.front(), 1);
    config << start << "\n";
    render_feet(start, &bmp, 0);
    for (auto i = 1; i < rrt.path.size() - 2; i++) {
        Feet feet;
        if (rrt.path[i].left_to_up.x == rrt.path[i + 1].left_to_up.x &&
            rrt.path[i].left_to_up.y == rrt.path[i + 1].left_to_up.y)
        {
            feet = Feet(rrt.path[i], 1);
            config << feet << "\n";
        } else {
            feet =  Feet(rrt.path[i], 0);
            config << feet << "\n";
        }
        render_feet(feet, &bmp, 0);
        //render_contour(rrt.path[i], &bmp, 0);
    }
    Feet finish(rrt.path.back(), 1);
    render_feet(finish, &bmp, 0);
    config << finish;

    bmp.out_bmp("MAP_PATH.bmp"); 
    return 0;
}

/*
имена некоторых переменных поясняются с учетом вчерашнего рисунка
буду пояснять некоторые действия, на тот случай, если я где-то описался или обсчитался

// ВАЖНОЕ ЗАМЕЧАНИЕ
// в файлике feet.config каждая строчка - координата левого ниженего каждодй стопы
// первая строчка - левая, вторая - правая, потом пустая строка, потом следующая пара
// x1, y1, z1 - координаты хипджойнта
// x, y, z - координаты точки крепления голени к стопе
// hip_length - длина бедра
// shin_length - длина голени

double OC = sqrt(pow(x1, 2) + pow(y1, 2));
double OD = sqrt(pow(x, 2) + pow(y, 2));
double DC = abs(OC - OD); //теперь есть катет прямоугольного прямоугольника ADC
double DA = z; // другой катет треугольника ADC
double AC = sqrt(pow(DA, 2) + pow(DC, 2)); // гипотенуза ADC
double dCa = asin(DA/DC);
double P_div = (AC + hip_length + shin_length) / 2.0;
double S = sqrt(P_div*(P_div - AC)*(P_div - hip_length)*(P_div - shin_length)); // герон
double aCb = asin(2*S / (AC * shin_length));
    double alpha = 180 - dCa - aCb; // внешний угол крепления голени к стопе
double dAc = asin(DC / AC);
double cAb = asin(2*S / (AC * hip_length));
    double phi = 90 - dAc - cAb; // бедро к плоскости стоп, если нога передняя
    double phi = 90 - cAb; // бедро к плоскости стоп, если нога задняя, вроде
double OE = y;
    double theta = acos(OE / OD); //бедро к ZX
    //третий угол константный для ходьбы, кажется

// ВАЖНО
 сейчас считается путь по прямой, если хочется посмотреть на поворотах, закомментируйте 24 строку
 и раскомментируйте 25


*/
