#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <algorithm>
#include <ctime>

static std::vector <Coordinates> go_path;
static Coordinates MovableObject(2, 2, 90, 40);
static Bitmap bmp(width, height);
static Map map;

void make_map() {
    map.height = height;
    map.width = width;
    map.obstacles.push_back(Obstacle(Coordinates(95, 0), Coordinates(105, 100)));
    /*map.points.push_back(Coordinates(45, 36, 350, 60));
    map.points.push_back(Coordinates(30, 30, 110, 60));*/

    map.points.push_back(Coordinates(2, 2, 90, 40));
    map.points.push_back(Coordinates(198, 2, 90, 40));

    /*map.points.push_back(Coordinates(20, 35, 190, 20));
    map.points.push_back(Coordinates(30, 30, 270, 20));*/

   /* map.points.push_back(Coordinates(30, 30, 355, 60));
    map.points.push_back(Coordinates(30, 30, 95, 60));*/


    /*map.points.push_back(Coordinates(45, 30, 55, 60));
    map.points.push_back(Coordinates(30, 36, 110, 60));*/
    /*map.points.push_back(Coordinates(45, 36, 55, 60));
    map.points.push_back(Coordinates(30, 30, 110, 60));*/


    /*map.points.push_back(Coordinates(35, 20, 55, 60));
    map.points.push_back(Coordinates(20, 26, 80, 60));*/
    /*map.points.push_back(Coordinates(35, 26, 55, 60));
    map.points.push_back(Coordinates(20, 20, 80, 60));*/

    /*map.points.push_back(Coordinates(35, 20, 120, 60));
    map.points.push_back(Coordinates(20, 26, 80, 60));*/
    /*map.points.push_back(Coordinates(35, 26, 120, 60));
    map.points.push_back(Coordinates(20, 20, 80, 60));*/
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
/*

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
*/

void move() {
    static long long i = 0;

    if (i != go_path.size()) {
        std::vector <Coordinates> current_coords;
        map.points.clear();
        render_map(map, &bmp);
        MovableObject.x = go_path[i].x;
        //std::cout << go_path[i].x << "\n";
        MovableObject.y = go_path[i].y;

        int end_MovableObject_x = (MovableObject.x + MovableObject.length * cos(MovableObject.phi * M_PI / 180));
        int end_MovableObject_y = MovableObject.y + MovableObject.length * sin(MovableObject.phi * M_PI / 180);

        current_coords.push_back(MovableObject);
        current_coords.push_back(Coordinates(end_MovableObject_x, end_MovableObject_y));

        render_path(current_coords, &bmp, 0);
        bmp.out_bmp("MAP_PATH.bmp");

        i++;
        glutPostRedisplay();
    }

}
/*

void timer(int value) {
    move();
    display();
    glutTimerFunc(5, timer, 0);
}
*/

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

/*
    bool ** temp_plane = new bool * [width];
    for (size_t i = 0; i < width; i++) {
        temp_plane[i] = new bool [height];
    }
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            temp_plane[i][j] = false;
        }
    }


    std::cout << map.points.front().x << " "
              << map.points.front().y << " "
            << map.points.front().length << " "
              << map.points.front().phi << "\n";

    std::cout << map.points.back().x << " "
              << map.points.back().y << " "
            << map.points.back().length << " "
              << map.points.back().phi << "\n";


    bresenham_obj(temp_plane, map.points.front(), map.points.back());*/



    RrTree rrt(&map, 500);

/*
    KdTree kd;
    kd.nodes.push_back(KdNode(&map.points.front().coords, 0));*/

    /*
    KdTree kd;
    kd.nodes.push_back(KdNode(&map.points.front().coords, 0));
    map.points.push_back(Coordinates(5, 6));
    //std::cout << rrt.is_available(&map, rrt.nodes.back().point, rrt.goal_state.point);
    std::cout << rrt.nodes.size() << std::endl;
    rrt.extend(&map, &kd, 1);
    std::cout << rrt.nodes.size() << std::endl;
*/

/*
    Coordinates one(2, 2);
    Coordinates two(198, 2);
    std::cout << rrt.is_available(&map, one, two);*/



    rrt.search(&map, 1);
    rrt.get_path(rrt.nodes.size() - 1);
    rrt.optimize_path(&map, 3, 20);
    std::reverse(go_path.begin(), go_path.end());


    /*for (int i = 0; i < rrt.path.size(); i++) {
        std::cout << rrt.path[i].x << " " << rrt.path[i].y << " " << rrt.path[i].phi << std::endl;
    }*/

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

}