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
        ratio1 = ratio1 / (width/2);
        ratio2 = (height / (rows + 2));
        ratio2 = ratio2 / (height/2);


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
        glLineWidth(1);


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

        float ratio = globalRatio;
        float x = -1.0 + (i + 1) * ratio;
        float y = 1.0 - ratio * (minSize + 1) + j * ratio;
        glColor3f(0.64, 0.64, 0.64);
        glPointSize(10);

        float radius = 0.04;
        int nrTriangles = 40;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int index = 0; index <= nrTriangles; index++) {
            glVertex2f(x + (radius * cos(index * 2.0f * 3.14 / nrTriangles)), y + (radius * sin(index * 2.0f * 3.14 / nrTriangles)));
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
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2d(x0, y0);
        glVertex2d(xn, yn);
        glEnd();
        float m = (yn - y0) / (xn - x0);
        float n = 0;
        float y = y0;
        for (float x = x0; x < xn; x = x + 0.1) {
            writePixel(x * 10 + 10, round(y + 0.5) + 1);
            y = y + m;
        }
    }
    void drawPixelsCircle(float xStart, float yStart, float radius) {
        float ratio = globalRatio;
        int radius2 = radius;
        radius = radius /10;

        float x0 = -1.0 + (xStart + 2) * ratio;
        float y0 = 1.0 - ratio * (minSize ) + yStart * ratio;

        glColor3f(1, 0.1, 0.1);
        glLineWidth(3);
        int lineAmount = 100; 

        glBegin(GL_LINE_LOOP);
        for (int index = 0; index <= lineAmount; index++) {
            glVertex2f(
                x0 + (radius * cos(index * 2.0f * 3.14 / lineAmount)),
                y0 + (radius * sin(index * 2.0f * 3.14 / lineAmount))
            );
          
        }
        glEnd();
        int x = 0, y = radius2;
        float d = 5.0 / 4 - radius2;
        writePixel(x + 5, y-2);
        while (y > x) {
            if (d < 2) {
                d += 2 * x + 3;
                x++;
            }
            else {
                d += 2 * (x - y) + 5;
                x++;
                y--;
            }
            writePixel(x + 5, y-2);
        }

    }
};

void Display1() {
    CartesianGrid cg;
    cg.CreateGrid(17, 17);
    cg.drawPixelsLine(0.0, 0.0, 17.0, 7.0);
    cg.drawPixelsLine(0.0, 16.0, 17.0, 11.0);
}

void Display2() {
    CartesianGrid cg1;
    cg1.CreateGrid(17, 17);
    cg1.drawPixelsCircle(0.0, 0.0,13.0);
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
    default:
        break;
    }
    glFlush();
}


void Reshape(int w, int h) {
    printf("Call Reshape : width = %d, height = %d\n", w, h);
    width = w;
    height = h;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    printf("You have pressed <%c>. The mouse has the position %d, %d.\n",
        key, x, y);

    prevKey = key;
    if (key == 27)
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