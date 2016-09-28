/*#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glut.h>

double ro = 6.f, phi = 0.01, theta = 0.01;

void changeSize(int w, int h){

    if(h == 0){
        h = 1;
    }

    float ratio = 1.0 * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

    gluPerspective(90,ratio,1,100);
}

void processKeys(unsigned char key, int x, int y){
    switch(key){
        case 'w': theta += 0.1f; break;
        case 's': theta -= 0.1f; break;
        case 'a': phi += 0.1f; break;
        case 'd': phi -= 0.1f; break;
        case 't': ro += 0.3f; break;
        case 'g': ro -= 0.3f; break;
        case 27: exit(0);
    }

}

int main(int argc, char** argv){
    int window;

    //Initializing GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);

    window = glutCreateWindow("Example 1");

    //Setting bg color
    glClearColor(1.0f,1.0f,1.0f,1.0f);

    //Registering callback function
    glutReshapeFunc(changeSize);

};*/

#include <GL/gl.h>
#include <GL/glut.h>

void display () {

    /* clear window */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw scene */
    glutSolidTeapot(.5);

    /* flush drawing routines to the window */
    glFlush();

}

int main ( int argc, char * argv[] ) {

    /* initialize GLUT, using any commandline parameters passed to the
       program */
    glutInit(&argc,argv);

    /* setup the size, position, and display mode for new windows */
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB);

    /* create and set up a window */
    glutCreateWindow("hello, teapot!");
    glutDisplayFunc(display);

    /* tell GLUT to wait for events */
    glutMainLoop();
}