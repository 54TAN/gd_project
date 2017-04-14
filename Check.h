#ifndef CHECK_H
#define CHECK_H

#include "Map.h"

class Check {
public:
    static bool check_brick(Map* map, Coordinates, Coordinates);
    static bool check_slice(Map* map, Coordinates, Coordinates);
    static bool check_line(Map* map, Coordinates, Coordinates);
};

#endif
