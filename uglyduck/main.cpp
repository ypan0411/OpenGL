#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

double ro=6.f, phi=0.01, theta=0.01;
void changesize(int w,int h);
void display();
void processkeys(unsigned char, int, int);




int main ( int argc, char * argv[] ) {

    /* initialize GLUT, using any commandline parameters passed to the
     program */
    glutInit(&argc,argv);

    int window;

    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);

    /* setup the size, position, and display mode for new windows */
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB);

    /* create and set up a window */
    window=glutCreateWindow("hello duuuuuuucccccckkkk");

    //backgroud colour
    glClearColor(1.0f, 1.0f, 0.0f, 0.0f);

    glutReshapeFunc(changesize);
    glutDisplayFunc(display);
    glutKeyboardFunc(processkeys);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(90.0, 1.0, 1.0, 1000.0);

    /* tell GLUT to wait for events */
    glutMainLoop();

}

void display () {

    double x=0.f,y=0.f,z=0.f;
    /* clear window */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    z=ro*cos(phi)*sin(theta);
    x=ro*sin(phi)*sin(theta);
    y=ro*cos(theta);

    //initialise modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //look from the origin of the coordinate system
    gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0*sin(theta), 0.0);


    glPushMatrix();
    glTranslatef(0.0,0.0,0.0);
    glColor3f(0.0,0.0,1.0);
    glScalef(3.0,2.0,4.0);
    glutSolidSphere(1,20,20);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0,3.0,-1.6);
    glColor3f(0.0,0.0,1.0);
    glScalef(1.5,1.5,1.5);
    glutSolidSphere(1,20,20);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0,2.0,-3.0);
    glColor3f(1.0, 0.5, 0.0);
    glScalef(0.6,0.3,0.8);
    glutSolidSphere(1,20,20);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.5,3.5,-3.0);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.2,0.2,0.2);
    glutSolidSphere(1,20,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5,3.5,-3.0);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.2,0.2,0.2);
    glutSolidSphere(1,20,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,0.0,4.0);
    glColor3f(0.1, 0.1, 0.2);
    glScalef(0.3,0.3,0.3);
    glutSolidSphere(1,20,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,-2.0,0.0);
    glColor3f(0.0,1.0,0.0);
    glScalef(5.0,0.3,5.0);
    glutSolidCube(3.0);
    glPopMatrix();









    /* flush drawing routines to the window */
    glFlush();
    glutSwapBuffers();

}

void changesize(int w,int h){
    if(h==0){
        h=1;
    }
    float ratio= 1.0* w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    gluPerspective(90, ratio, 1, 1000);

}


void processkeys(unsigned char key, int x, int y){
    switch (key) {
        case 'w':
            theta+=0.1f;
            break;
        case 's':
            theta-=0.1f;
            break;
        case 'a':
            phi+=0.1f;
            break;
        case 'd':
            phi-=0.1f;
            break;
        case 't':
            ro+=0.3f;
            break;
        case 'g':
            ro-=0.3f;
            break;
    }

    while (theta>2*M_PI) theta-=2*M_PI;
    while (theta<0) theta+=2*M_PI;
    while (phi>2*M_PI) phi-=2*M_PI;
    while (phi<0) phi+=2*M_PI;

    glutPostRedisplay();
}