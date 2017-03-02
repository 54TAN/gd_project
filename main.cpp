#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <cmath>
#include <ctime>

#define _USE_MATH_DEFINES

std::vector <Coordinates> go_path;
Coordinates MovableObject(2, 498, (-3/2.0) * M_PI, 50);

unsigned int width = 1000;
unsigned int height = 500;

void make_map(int width, int height, Map & map) {
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Coordinates(380, 200), Coordinates(390, 500)));
    map.obstacles.push_back(Obstacle(Coordinates(780, 200), Coordinates(790, 500)));
    map.obstacles.push_back(Obstacle(Coordinates(580, 0), Coordinates(590, 300)));
    map.points.push_back(Coordinates(2, 498));
    map.points.push_back(Coordinates(998, 498));
}

void display(void) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned char header[54];
    unsigned char palette[1024];
    unsigned int imageSize = width * height * 3;
    unsigned char * data = new unsigned char [imageSize];

    FILE * file = fopen("MAP_PATH.bmp", "rb");
    fread(header, 1, 54, file);
    fread(palette, 1, 1024, file);
    fread(data, 1, imageSize, file);
    fclose(file);

    glDrawPixels(width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
    glutSwapBuffers();
}

Bitmap bmp(width, height);
Map map;

void move() {
    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
    std::vector <Coordinates> current_coords;
    MovableObject.x += 3;

    current_coords.push_back(MovableObject);
    current_coords.push_back(Coordinates(MovableObject.x, MovableObject.y - MovableObject.length));
    render_path(current_coords, &bmp, 0);
    bmp.out_bmp("MAP_PATH.bmp");
    glutPostRedisplay();
}

void timer(int value) {
    move();
    display();
    glutTimerFunc(5, timer, 0);
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(move);
            break;
    }
}

int main(int argc, char ** argv) {

    srand(time(NULL));


    make_map(width, height, map);
    //Bitmap bmp(map.width, map.height);
    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");

/*
    RrTree rrt(&map, 2000);
    rrt.search(&map, 1);
    rrt.get_path(rrt.nodes.size() - 1);
    rrt.optimize_path(&map, 3, 10);

    go_path = rrt.path;
*/


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    //glutTimerFunc(1, timer, 0);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}