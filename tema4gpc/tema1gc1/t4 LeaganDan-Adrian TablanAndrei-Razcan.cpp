#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <assert.h>
#include <float.h>
#include <iostream>

#include "glut.h"

unsigned char prevKey;
int width;
int height;

class CartesianGrid {
private:
    int minSize = 0;
    float globalRatio = 0;

public:
    CartesianGrid() {}
    void CreateGrid(int rows, int cols) {
        minSize = std::min(width, height);
        minSize = rows - 1;
        float ratio1 = 0.0;
        float ratio2 = 0.0;
        ratio1 = (width / (cols + 2));
        ratio1 = ratio1 / 150;
        ratio2 = (height / (rows + 2));
        ratio2 = ratio2 / 150;


        glColor3f(0.0, 0.0, 0.0);
        float y = 1.0;
        float x = -1.0;
        float ratio = 0.0;
        if (width < height) {
            ratio = ratio1;
        }
        else {
            ratio = ratio2;
        }
        globalRatio = ratio;

        for (int indexRow = 0; indexRow < rows; indexRow += 1) {
            y = y - ratio;
            glBegin(GL_LINES);
            glVertex2d(-1.0 + ratio, y);
            glVertex2d(-1.0 + rows * ratio, y);
            glEnd();

        }
        for (int indexCol = 0; indexCol < cols; indexCol += 1) {
            x = x + ratio;
            glBegin(GL_LINES);
            glVertex2d(x, 1.0 - ratio);
            glVertex2d(x, 1.0 - cols * ratio);
            glEnd();

        }
    }
    void writePixel(float i, float j) {

        printf("%f %f\n", i, j);

        float ratio = globalRatio;

        float x = -1.0 + (i + 1) * ratio;
        float y = 1.0 - ratio * (minSize + 1) + j * ratio;
        glColor3f(0.64, 0.64, 0.64);
        glPointSize(10);

        float radius = 0.04;
        int triangleAmount = 40;
        float twice_Pi = 2.0f * 3.14;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int index = 0; index <= triangleAmount; index++) {
            glVertex2f(x + (radius * cos(index * twice_Pi / triangleAmount)), y + (radius * sin(index * twice_Pi / triangleAmount)));
        }
        glEnd();
    }
    void drawPixelsLine(float xStart, float yStart, float xEnd, float yEnd) {
        float ratio = globalRatio;

        float x0 = -1.0 + (xStart + 1) * ratio;
        float y0 = 1.0 - ratio * (minSize + 1) + yStart * ratio;
        float xn = -1.0 + xEnd * ratio;
        float yn = 1.0 - ratio * (minSize + 1) + yEnd * ratio;
        glColor3f(1, 0.1, 0.1);
        glBegin(GL_LINES);
        glVertex2d(x0, y0);
        glVertex2d(xn, yn);
        glEnd();
        float m = (yn - y0) / (xn - x0);
        float n = 0;
        float y = y0;
        for (float x = x0; x < xn; x=x+0.1) {
            y = y + m;
            writePixel(x, round(y));
        }
        float x = x0;
        float m_inverse = 1.0 / m;
        for (float y = y0; y < yn; y=y+0.1) {
            x = m_inverse + x;
            writePixel(round(x), y);
        }

    }
};

void Display1() {
    CartesianGrid cg;
    cg.CreateGrid(17, 17);
    cg.drawPixelsLine(0.0, 0.0, 17.0, 7.0);
}
void Display10() {
    glColor3f(0.2, 0.15, 0.88);

    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(-1, -1);
    glEnd();

    glColor3f(1, 0.1, 0.1); // red
    // A line segment is drawn between the points
    // specified by the vertices 
    glBegin(GL_LINES);
    glVertex2i(-1, 1);
    glVertex2i(1, -1);
    glEnd();

    // A line segment is drawn between the points
    // specified by the vertices 
    glBegin(GL_LINES);
    glVertex2d(-0.5, 0);
    glVertex2d(0.5, 0);
    glEnd();
}

void Display2() {
    glColor3f(1, 0.1, 0.1); // red
    // In this case, by using GL_LINES begin/end paradigm
    // several line segment are drawn, between the 1st and
    // the 2nd point, between the 3rd and the 4th, etc.  
    glBegin(GL_LINES);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.9, 0.9);
    glVertex2f(0.8, 0.8);
    glVertex2f(0.7, 0.7);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-1.0, -1.0);
    glEnd();
}

void Display3() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_POINTS);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}

void Display4() {
    // The geometry object in this case is a polygonal line.

    // By using GL_LINE_STRIP begin/end paradigm 
    // it is possible to draw a polygonal line having as many 
    // points as there are specified between glBegin and glEnd.

    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_LINE_STRIP);
    glVertex2f(1, 1);
    glVertex2f(1, 0.84);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.6, 0.44);
    glEnd();
}

void Display5() {
    // The geometry object in this case is a closed polygonal line.

    // By using GL_LINE_LOOP begin/end paradigm 
    // it is possible to draw a closed polygonal line having as many 
    // points as there are specified between glBegin and glEnd.

    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_LINE_LOOP);
    glVertex2f(1, 1);
    glVertex2f(1, 0.84);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.6, 0.44);
    glEnd();
}

void Display6() {
    // The geometry object in this case is a triangle.

    // By using GL_TRIANGLES begin/end paradigm 
    // it is possible to draw triangles having the vertices
    // (v0,v1,v2), (v3,v4,v5), ...
    // There should be specified at least 3 vertices between
    // glBegin and glEnd.

    glColor3f(1, 0.1, 0.1); // red
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    // add lines of code here
    glVertex2f(1, 1);
    glVertex2f(1, 0.84);
    glVertex2f(0.84, 0.84);
    glVertex2f(-1, -1);
    glVertex2f(-1, -0.84);
    glVertex2f(-0.84, -0.84);
    glEnd();
}

void Display7() {


    glColor3f(1, 0.1, 0.1); // red
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(1, 1);
    glVertex2f(0.3, 0.7);
    glVertex2f(0.5, 0.5);
    glVertex2f(1, 0.5);
    glEnd();
}

void Display8() {


    glColor3f(0.1, 0.1, 1); // blue
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f(0.75, 0);
    glVertex2f(0.4, 0.65);
    glVertex2f(-0.4, 0.65);
    glVertex2f(-0.75, 0);
    glVertex2f(-0.4, -0.65);
    glVertex2f(0.4, -0.65);
    glEnd();

    glColor3f(1.0, 1.0, 1.0); //white
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f(0.5, 0);
    glVertex2f(0.25, 0.45);
    glVertex2f(-0.25, 0.45);
    glVertex2f(-0.5, 0);
    glVertex2f(-0.25, -0.45);
    glVertex2f(0.25, -0.45);
    glEnd();

    glColor3f(1, 0.1, 0.1); // red
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex2f(0.5, 0);
    glVertex2f(0.25, 0.45);
    glVertex2f(-0.25, 0.45);
    glVertex2f(-0.5, 0);
    glVertex2f(-0.25, -0.45);
    glVertex2f(0.25, -0.45);
    glEnd();

}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(1);
    glPointSize(2);
    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    printf("Call Display\n");

    // The buffer GL_COLOR_BUFFER_BIT is cleared
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        //un singur apel cu 6 varfuri
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        //3 apeluri
        //separat perfect, sa nu treaca linii albastre, rosii
        //pixelul care a fost desenat mai tarziu acopera pixelul desenat inainte

    default:
        break;
    }

    // practically, this call forces the redrawing of 
    // the application window
    glFlush();
}


void Reshape(int w, int h) {
    printf("Call Reshape : width = %d, height = %d\n", w, h);
    width = w;
    height = h;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

// The key parameter indicates the key code and x, y
// the position of the mouse cursor
void KeyboardFunc(unsigned char key, int x, int y) {
    printf("You have pressed <%c>. The mouse has the position %d, %d.\n",
        key, x, y);

    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
    printf("Call MouseFunc : you have %s the %s button in the position %d %d\n",
        (state == GLUT_DOWN) ? "pressed" : "released",
        (button == GLUT_LEFT_BUTTON) ?
        "left" :
        ((button == GLUT_RIGHT_BUTTON) ? "right" : "middle"),
        x, y);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(300, 300);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);


    glutMainLoop();

    return 0;
}