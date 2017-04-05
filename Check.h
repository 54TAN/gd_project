#ifndef CHECK_H
#define CHECK_H

#include "Map.h"

class Check {
public:
    static bool check_brick(Map *map, Coordinates object_1, Coordinates object_2);
    static bool check_slice(Map *map, Coordinates object_1, Coordinates object_2);
};

#endif
