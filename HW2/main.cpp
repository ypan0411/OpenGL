#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;
#define DEBUG

double ro = 10.f, phi = 0.0f, theta = 0.f;
double r_shift = 0.f, f_shift = 0.f;
double x = 0.f, y = 0.f, z = 10.f;
double x_shift = 0.f;
double z_shift = 0.f;
int degree = 0;

void changeSize(int w, int h){
    if(h == 0){
        h = 1;
    }

    float ratio = 1.0 * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

    gluPerspective(45,ratio,1,100);
}

void change_coordinate(){
    z = ro*cos(theta)*cos(phi);
    x = ro*sin(phi)*cos(theta);
    y = ro*sin(theta);
}

void change_duck_position(){
    x_shift += r_shift*cos(phi) + f_shift*sin(phi);
    z_shift += r_shift*sin(-phi) + f_shift*cos(phi);
}

void processKeys(unsigned char key, int x, int y){
    switch(key){
        case 'w': theta += 0.05f;
            change_coordinate();
            break;
        case 's': theta -= 0.05f;
            change_coordinate();
            break;
        case 'a': phi -= 0.1f;
            change_coordinate();
            break;
        case 'd': phi += 0.1f;
            change_coordinate();
            break;
        case 't': ro += 0.3f;
            change_coordinate();
            break;
        case 'g': ro -= 0.3f;
            change_coordinate();
            break;
        case 'j':
            r_shift = -0.3f;
            f_shift = 0.f;
            change_duck_position();
            break;
        case 'l':
            r_shift = 0.3f;
            f_shift = 0.f;
            change_duck_position();
            break;
        case 'i':
            f_shift = -0.3f;
            r_shift = 0.f;
            change_duck_position();
            break;
        case 'k':
            f_shift = 0.3f;
            r_shift = 0.f;
            change_duck_position();
            break;
//        case 'y': degree += 10;
//            change_duck_position();
//            break;
//        case 'h': degree -= 10;
//            change_duck_position();
//            break;
        case 27: exit(0);
    }

    while(theta > 2*M_PI){
        theta -= 2*M_PI;
    }
    while(theta < 0){
        theta += 2*M_PI;
    }
    while(phi > 2*M_PI){
        phi -= 2*M_PI;
    }
    while(phi < 0){
        phi += 2*M_PI;
    }
    while(degree > 360){
        degree -= 360;
    }
    while(degree < 0){
        degree += 360;
    }
    glutPostRedisplay();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#ifdef DEBUG
    cout << theta << " " << phi << endl;
    cout << x << " " << y << " " << z << endl;
    cout << x_shift << " " << z_shift << endl;
#endif

    gluLookAt(x, y, z, -x, -y, -z, 0.0, 1.0*cos(theta), 0.0);
    /*if(theta < 0.5*M_PI && theta >= 0.0){
        gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else if(theta < 0.0){
        gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else{
        gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
    }*/



    //body
    glLineWidth(1.5f);
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f+x_shift, 0.0f, 0.0f+z_shift);
//    glRotatef(degree, 0.0f, 0.0f, 1.0f);
    glScalef(2.0f, 1.0f, 1.5f);
    glutSolidSphere(1.0, 100, 100);
    glPopMatrix();


    //head
    glLineWidth(1.5f);
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(-1.8f+x_shift, 1.0f, 0.0f+z_shift);
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidSphere(1.0, 100, 100);
    glPopMatrix();


    //tail
    glLineWidth(1.5f);
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushMatrix();
//    glRotatef(degree, 0.0f, 0.0f, 1.0f);
    glTranslatef(2.0f+x_shift, 0.3f, 0.0f+z_shift);
    glScalef(0.5f, 0.5f, 0.5f);
    glutSolidSphere(1.0, 100, 100);
    glPopMatrix();

    //beak
    glLineWidth(1.5f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glPushMatrix();
//    glRotatef(degree, 0.0f, 0.0f, 1.0f);
    glTranslatef(-2.7f+x_shift, 1.0f, 0.0f+z_shift);
    glRotated(-90, 0, 1, 0);
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidCone(0.25, 0.4, 100, 100);
    glPopMatrix();

    //eyes
    glLineWidth(1.5f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
//    glRotatef(degree, 0.0f, 0.0f, 1.0f);
    glTranslatef(-2.4f+x_shift, 1.5f, -0.5f+z_shift);
    glScalef(0.15f, 0.15f, 0.15f);
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();

    glLineWidth(1.5f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
//    glRotatef(degree, 0.0f, 0.0f, 1.0f);
    glTranslatef(-2.4f+x_shift, 1.5f, 0.5f+z_shift);
    glScalef(0.15f, 0.15f, 0.15f);
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();

    //water
    glLineWidth(1.5f);
    glColor3f(0.0f, 0.75f, 1.0f);

    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(40.f, 0.5f , 40.f);
    glutSolidCube(1.0);
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv){

    //Initializing GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);

    glutCreateWindow("Jacobs Duck");

    glEnable(GL_DEPTH_TEST|GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_TEST|GL_DEPTH_BUFFER_BIT);

    //Setting bg color
    glClearColor(0.68f,1.0f,1.0f,1.0f);

    //Registering callback function
    glutReshapeFunc(changeSize);
    glutDisplayFunc(display);
    glutKeyboardFunc(processKeys);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 1000.0);

    glutMainLoop();
    return 0;

};