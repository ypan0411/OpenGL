#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;
double x = 0.f, y = 0.f, z = 10.f;
bool smooth_button = true;
bool light0 = true;
bool light1 = false;
float diffuse = 1.0f;
float specular = 0.0f;
float shininess = 32.0f;
float ambient = 0.2f;

void init(void)
{
    GLfloat mat_specular[] = { specular, specular, specular, 1.0 };
    GLfloat mat_diffuse[] = {diffuse, diffuse, diffuse, 1.0};
    GLfloat mat_shininess[] = { shininess };
    GLfloat mat_ambient[] = {ambient, ambient, ambient, 1.0};
    GLfloat mat_diffuse1[] = {0.0, diffuse, diffuse, 1.0};
    GLfloat light_position0[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_position1[] = {-1.0f, 5.0, 4.0, 1.0};
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_LIGHTING);
    if (smooth_button) glShadeModel (GL_SMOOTH);
    else glShadeModel(GL_FLAT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //light0
    if(light0) {
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    }
    else{
        glDisable(GL_LIGHT0);
    }
    //light1
    if(light1) {
        glLightfv(GL_LIGHT1, GL_SPECULAR, mat_specular);
        glLightfv(GL_LIGHT1, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, mat_diffuse1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, mat_ambient);
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    }
    else {
        glDisable(GL_LIGHT1);
    }

    glEnable(GL_DEPTH_TEST);
}

void display(){
    init ();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(x, y, z, -x, -y, -z, 0.0, 1.0, 0.0);

    glLineWidth(1.5f);

    glPushMatrix();
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidSphere(3.0, 100, 100);
    glPopMatrix();

    glutSwapBuffers();
}

void processKeys(unsigned char key, int x, int y){
    switch(key){
        case 's': smooth_button = !smooth_button; break;
        case 'a': light0 = !light0; break;
        case 'd': light1 = !light1; break;
        case 'u': if(diffuse <= 0.95f) diffuse += 0.05f; break;
        case 'j': if(diffuse >= 0.05f) diffuse -= 0.05f; break;
        case 'i': if(ambient <= 0.95f) ambient += 0.05f; break;
        case 'k': if(ambient >= 0.05f) ambient -= 0.05f; break;
        case 'o': if(specular <= 0.95f) specular += 0.05f; break;
        case 'l': if(specular >= 0.05f) specular -= 0.05f; break;
        case 'y': if(shininess <= 512.0f) shininess += 1.0f; break;
        case 'h': if(shininess >= 2.0f) shininess -= 1.0f; break;
        case 27: exit(0);
    }

    glutPostRedisplay();
}

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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processKeys);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 1000.0);
    glutMainLoop();
    return 0;
}