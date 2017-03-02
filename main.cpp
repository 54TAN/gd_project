#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <cmath>
#include <ctime>

unsigned int width = 1000;
unsigned int height = 500;

static GLfloat spin = 0.0;

void make_map(int width, int height, Map & map) {
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Coordinates(380, 200), Coordinates(390, 500)));
    map.obstacles.push_back(Obstacle(Coordinates(780, 200), Coordinates(790, 500)));
    map.obstacles.push_back(Obstacle(Coordinates(580, 0), Coordinates(590, 300)));
    map.points.push_back(Coordinates(2, 498));
    map.points.push_back(Coordinates(998, 498));
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

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
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

void timf(int value) {
    Map map;
    make_map(width, height, map);
    Bitmap bmp(map.width, map.height);
    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");

    RrTree rrt(&map, 200);
    //rrt.search(&map, 1, &bmp);

    double temp[2] = {map.points.front().x, map.points.front().y};
    KdTree kd;
    kd.nodes.push_back(KdNode(temp, 0));
    while (!rrt.is_available(&map, rrt.nodes.back().point, rrt.goal_state.point)) {
        map.generate_points(1, map.width, map.height);
        rrt.extend(&map, &kd, true, &bmp);
        display();
    }

    rrt.get_path(rrt.nodes.size() - 1);
    render_path(rrt.path, &bmp, 1);
    bmp.out_bmp("MAP_PATH.bmp");
    glutPostRedisplay();
    rrt.optimize_path(&map, 3, 10);
    render_path(rrt.path, &bmp, 1, 1);
    bmp.out_bmp("MAP_PATH.bmp");
    glutPostRedisplay();  // Redraw windows

    //glutTimerFunc(40, timf, 0); // Setup next timer
}


int main(int argc, char ** argv) {

    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutTimerFunc(4, timf, 0);
    glutMainLoop();

    return 0;
}