#include "KdNode.h"
#include "KdTree.h"

#include <iostream>

int main(int argc, char ** argv) {

    KdTree kd;
    kd.input("data.txt");
    std::ofstream file;
    file.open("out.gv");
    kd.save(file);

    return 0;
}