#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <algorithm>
#include <ctime>

static std::vector <Coordinates> go_path;
static Coordinates MovableObject(2, 60, 90, 60);
static Bitmap bmp(width, height);
static Map map;

void make_map() {
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Coordinates(95, 0), Coordinates(105, 100)));
    map.points.push_back(Coordinates(2, 60, 90, 60));
    map.points.push_back(Coordinates(198, 60, 270, 60));
    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    delete data;
}

void rrt_in_window() {
    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");

    RrTree rrt(&map, 20);

    double temp[2] = {map.points.front().x, map.points.front().y};
    KdTree kd;
    kd.nodes.push_back(KdNode(&map.points.front().coords, 0));
    while (!rrt.is_available(&map, rrt.nodes.back().point, rrt.goal_state.point)) {
        map.generate_points(1, map.width, map.height, 60);
        rrt.extend(&map, &kd, true, &bmp);
        display();
    }

    rrt.get_path(rrt.nodes.size() - 1);
    render_path(rrt.path, &bmp, 1);
    bmp.out_bmp("MAP_PATH.bmp");
    display();
    rrt.optimize_path(&map, 3, 10);
    render_path(rrt.path, &bmp, 1, 1);
    bmp.out_bmp("MAP_PATH.bmp");
    display();
}

void move() {
    static long long i = 0;

    if (i != go_path.size()) {
        std::vector <Coordinates> current_coords;
        map.points.clear();
        render_map(map, &bmp);
        MovableObject.x = go_path[i].x;
        std::cout << go_path[i].x << "\n";
        MovableObject.y = go_path[i].y;
        current_coords.push_back(MovableObject);
        current_coords.push_back(Coordinates(MovableObject.x, MovableObject.y - MovableObject.length));
        render_path(current_coords, &bmp, 0);
        bmp.out_bmp("MAP_PATH.bmp");
        i++;
        glutPostRedisplay();
    }

}

void timer(int value) {
    move();
    display();
    glutTimerFunc(5, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(move);
            break;
    }
}

int main(int argc, char ** argv) {

    srand(time(NULL));


    make_map();

    RrTree rrt(&map, 500);
    rrt.search(&map, 1);
    std::cout << "sd";
    rrt.get_path(rrt.nodes.size() - 1);
    rrt.optimize_path(&map, 3, 10);
/*

    render_path(rrt.path, &bmp, 0, 0, true, &go_path);
    std::reverse(go_path.begin(), go_path.end());

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutTimerFunc(1, timer, 0);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
*/
}