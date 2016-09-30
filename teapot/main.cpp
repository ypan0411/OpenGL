#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>



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

        while(theta > 2*M_PI){
            theta -= 2*M_PI;
        }
        while(theta < 0){
            theta += 2*M_PI;
        }
        while(phi > 2*M_PI){
            phi -= 2*M_PI;
        }
        while(phi < 2*M_PI){
            phi += 2*M_PI;
        }
        glutPostRedisplay();
    }

    void display(){
        double x = 0.f, y = 0.f, z = 0.f;
        glClear(GL_COLOR_BUFFER_BIT);

        z = ro*cos(phi)*sin(theta);
        x = ro*sin(phi)*cos(theta);
        y = ro*cos(theta);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0*sin(theta), 0.0);

        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);

        glPushMatrix();
        glTranslatef(1.5f, 0.0f, 1.0f);
        glScalef(3.0f, 1.0f, 1.0f);
        glutWireCube(1.0);
        glPopMatrix();

        glLineWidth(1.5f);
        glColor3f(0.0f, 0.0f, 1.0f);

        glPushMatrix();
        glTranslatef(3.5f, 1.5f, 0.9f);
        glScalef(3.0f, 1.0f, 1.0f);
        glutWireCube(1.0);
        glPopMatrix();



        glutSwapBuffers();
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
        glutDisplayFunc(display);
        glutKeyboardFunc(processKeys);

        glMatrixMode(GL_PROJECTION);
        gluPerspective(90.0, 1.0, 1.0, 1000.0);

        glutMainLoop();
        return 0;

    };