#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <cmath>
#include <ctime>

void make_map(int width, int height, Map & map) {
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Point(380, 200), Point(390, 500)));
    map.obstacles.push_back(Obstacle(Point(780, 200), Point(790, 500)));
    map.obstacles.push_back(Obstacle(Point(580, 0), Point(590, 300)));
    map.points.push_back(Point(2, 498));
    map.points.push_back(Point(998, 498));
}

void out_info(RrTree const& rrt, Map const& map, clock_t time = -1) {
    int hours = time / 3600;
    int minutes = (time - hours * 3600) / 60;
    int seconds = time - hours * 3600 - minutes * 60;
    if (time != -1) {
        std::cout << "points map : " << map.points.size() << '\n';
        std::cout << "rrt points : " << rrt.nodes.size() << '\n';
        std::cout << "path points : " << rrt.path.size() << '\n';
        std::cout << "time : " << hours << ":" << minutes << ":" << seconds << std::endl;
    }
}

void make_picture(RrTree & rrt, Map const& map) {
    Bitmap my_bmp(map.width, map.height);
    render_map(map, &my_bmp);
    for (size_t i = 0; i < rrt.nodes.size(); i++) {
        rrt.go(i);
        if (rrt.edges.size())
            render_path(rrt.edges, &my_bmp, 0);
    }
    render_path(rrt.path, &my_bmp, 1);
    my_bmp.out_bmp("MAP_PATH.bmp");
}










static GLfloat spin = 0.0;

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glRotatef(spin, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glRectf(-25.0, -25.0, 25.0, 25.0);
    glPopMatrix();
    glutSwapBuffers();
}

/*
void spinDisplay(void)
{
    spin = spin + 2.0;
    if (spin > 360.0)
        spin = spin - 360.0;
    glutPostRedisplay();
}
*/

/*
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
*/

/*
void mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(spinDisplay);
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(NULL);
            break;
        default:
            break;
    }
}
*/

void process_in_window(int c, char ** v, Map & map) {
    RrTree my_rrt(&map, 100, 1);
    my_rrt.get_path(my_rrt.nodes.size() - 1);
    make_picture(my_rrt, map);

    glutInit(&c, v);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(map.width, map.height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(v[0]);
    init();
    glutDisplayFunc(display);
    //glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
    glutMainLoop();
}

int main(int argc, char ** argv) {

    srand(time(NULL));

    int width = 1000;
    int height = 500;

    Map my_map;
    make_map(width, height, my_map);

    process_in_window(argc, argv, my_map);
/*

    clock_t t = clock();

    RrTree my_rrt(&my_map, 5000, 1);
    my_rrt.get_path(my_rrt.nodes.size() - 1);

    t = (clock() - t) / CLOCKS_PER_SEC;

    out_info(my_rrt, my_map, t);
    make_picture(my_rrt, my_map);

*/
    return 0;
}