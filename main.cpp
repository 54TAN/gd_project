#include "KdTree.h"
#include "Map.h"
#include "RrTree.h"
#include "Render.h"

#include "GL/glut.h"

#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>

static std::vector <Coordinates> go_path;
static std::vector <Coordinates> replicated_path;
static Coordinates MovableObject(2, 2, 90, 60);
static Bitmap bmp(width, height);
static Map map;

void make_map() 
{
    map.height = height;
    map.width = width;
//	map.obstacles.push_back(Obstacle(Coordinates(95, 0), Coordinates(105, 60)));

//    map.points.push_back(Coordinates(2, 2, 90, 60));
//    map.points.push_back(Coordinates(198, 2, 90, 60));
    map.obstacles.push_back(Obstacle(Coordinates(295, 0), Coordinates(305, 400)));
    map.obstacles.push_back(Obstacle(Coordinates(695, 200), Coordinates(705, 600)));

    map.points.push_back(Coordinates(2, 2, 90, 60));
    map.points.push_back(Coordinates(998, 530, 90, 60));

    render_map(map, &bmp);
    bmp.out_bmp("MAP_PATH.bmp");
}
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

void move() {

    static long long i = 0;
    static int new_phi = MovableObject.phi;
    static int step = 1;

    if (i != go_path.size()) {
        std::vector <Coordinates> current_coords;
        //почистили
        map.points.clear();
        render_map(map, &bmp);


        MovableObject.x = go_path[i].x;
        //std::cout << go_path[i].x << "\n";
        MovableObject.y = go_path[i].y;
        if (go_path[i].phi != 0) new_phi = go_path[i].phi;

        if (new_phi < MovableObject.phi) step *= -1;
        if (new_phi > MovableObject.phi) step = abs(step);

        int end_MovableObject_x = (MovableObject.x + MovableObject.length * cos(MovableObject.phi * M_PI / 180));
        int end_MovableObject_y = MovableObject.y + MovableObject.length * sin(MovableObject.phi * M_PI / 180);

        current_coords.push_back(MovableObject);
        current_coords.push_back(Coordinates(end_MovableObject_x, end_MovableObject_y));

        render_path(current_coords, &bmp, 0);
        bmp.out_bmp("MAP_PATH.bmp");
        glutPostRedisplay();

        while (MovableObject.phi != new_phi) {
            int end_MovableObject_x = (MovableObject.x + MovableObject.length * cos(MovableObject.phi * M_PI / 180));
            int end_MovableObject_y = MovableObject.y + MovableObject.length * sin(MovableObject.phi * M_PI / 180);

            current_coords.push_back(MovableObject);
            current_coords.push_back(Coordinates(end_MovableObject_x, end_MovableObject_y));

            render_path(current_coords, &bmp, 0);
            MovableObject.phi += step;
            bmp.out_bmp("MAP_PATH.bmp");
            glutPostRedisplay();
        }

        new_phi = MovableObject.phi;

        i++;
        glutPostRedisplay();
    }

}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(move);
            break;
    }
}
*/
int main(int argc, char ** argv) 
{
    srand(time(NULL));

    make_map();

    RrTree rrt(&map, 500);
    rrt.search(&map, 1);
    rrt.get_path(rrt.nodes.size() - 1);
	Coordinates Movableobject(2, 2, 90, 60);
    Coordinates Previous(Movableobject);

/*	std::reverse(rrt.path.begin(), rrt.path.end());
	rrt.optimize_path(&map);

	std::reverse(go_path.begin(), go_path.end());
    replicated_path = rrt.path;

    render_path(rrt.path, &bmp, 0, 0, true, &go_path);
    std::reverse(go_path.begin(), go_path.end());
    //coordinates movableobject(2, 2, 90, 60);
    //coordinates previous(movableobject);
	std::reverse(go_path.begin(), go_path.end());

    for (int i = 0; i < replicated_path.size(); ++i) {
        std::cout << replicated_path[i].x << " " << replicated_path[i].y << " " << replicated_path[i].phi << "\n";
    }

    for (size_t i = 0; i < go_path.size(); i++) {
        for (int j = 0; j < replicated_path.size(); ++j) {
            if (go_path[i].x == replicated_path[j].x && go_path[i].y == replicated_path[j].y) {

                go_path[i] = replicated_path[j];

            }

            //std::cout << go_path[i].x << " " << go_path[i].y << " " << go_path[i].phi << "\n";
        }

    }
*/
    for (auto item : rrt.path) {
        Previous = MovableObject;

		MovableObject.x = item.x;
		MovableObject.y = item.y;
        
       	std::vector <Coordinates> current_coords;
        std::vector <Coordinates> heels;

		int end_MovableObject_x = (MovableObject.x + MovableObject.length * cos(item.phi * M_PI / 180));
        int end_MovableObject_y = MovableObject.y + MovableObject.length * sin(item.phi * M_PI / 180);

        current_coords.push_back(MovableObject);
        current_coords.push_back(Coordinates(end_MovableObject_x, end_MovableObject_y));

        heels.push_back(MovableObject);
        heels.push_back(Previous);

        render_path(current_coords, &bmp, 0);
        if (item != rrt.path.front())render_path(heels, &bmp, 1);
    }  

    bmp.out_bmp("MAP_PATH.bmp"); 
/*	glutInit(&argc, argv);
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
*/
    return 0;
}
