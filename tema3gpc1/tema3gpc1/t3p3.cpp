#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <assert.h>
#include <float.h>

#include "glut.h"

// the size of the window measured in pixels
#define dim 300
// the maximum number of iterations for the Julia-Fatou set membership testing 
#define NRITER_JF 5000
// the maximum value of M for the Julia-Fatou set membership testing 
#define MODMAX_JF 10000000
// increments used in CJuliaFatou
#define RX_JF 0.01
#define RY_JF 0.01
#define my_point_size (1.5)
#define my_step (0.007)

unsigned char prevKey;
int nivel = 0;


class CComplex {
public:
    CComplex() : re(0.0), im(0.0) {}
    CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
    CComplex(const CComplex& c) : re(c.re), im(c.im) {}
    ~CComplex() {}

    CComplex& operator=(const CComplex& c)
    {
        re = c.re;
        im = c.im;
        return *this;
    }

    double getRe() { return re; }
    void setRe(double re1) { re = re1; }

    double getIm() { return im; }
    void setIm(double im1) { im = im1; }

    double getModul() { return sqrt(re * re + im * im); }

    int operator==(CComplex& c1)
    {
        return ((re == c1.re) && (im == c1.im));
    }

    CComplex pow2()
    {
        CComplex rez;
        rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
        rez.im = 2.0 * re * im;
        return rez;
    }


    friend CComplex operator+(const CComplex& c1, const CComplex& c2);
    friend CComplex operator*(CComplex& c1, CComplex& c2);

    void print(FILE* f)
    {
        fprintf(f, "%.20f%+.20f i", re, im);
    }

private:
    double re, im;
};

CComplex operator+(const CComplex& c1, const CComplex& c2)
{
    CComplex rez(c1.re + c2.re, c1.im + c2.im);
    return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2)
{
    CComplex rez(c1.re * c2.re - c1.im * c2.im,
        c1.re * c2.im + c1.im * c2.re);
    return rez;
}

class CMandelbrot {
public:
    CMandelbrot()
    {
        // m.c is initialized by default with 0+0i

        m.nriter = NRITER_JF;
        m.modmax = MODMAX_JF;
    }

    CMandelbrot(CComplex& c)
    {
        m.c = c;
        m.nriter = NRITER_JF;
        m.modmax = MODMAX_JF;
    }

    ~CMandelbrot() {}

    void setmodmax(double v) { assert(v <= MODMAX_JF); m.modmax = v; }
    double getmodmax() { return m.modmax; }

    void setnriter(int v) { assert(v <= NRITER_JF); m.nriter = v; }
    int getnriter() { return m.nriter; }

    // it tests if x belongs to the Julia-Fatou set Jc
    // it returns 0 if yes, -1 for finite convergence, +1 for infinite convergence
    int isIn(CComplex& nr)
    {
        int rez = 0;
        // an array for storing the values for computing z_n+1 = z_n * z_n + c
        CComplex z0, z1;

        z0 = nr;
        CComplex c = z0;
        for (int i = 1; i < m.nriter; i++)
        {
            z1 = z0 * z0 + c;
            if (z1 == z0 || z1.getModul() > 2)
            {
                // x does not belong to the J-F set because the 
                // iterative process converges finitely
                rez = -1;
                break;
            }
            else if (z1.getModul() > m.modmax)
            {
                // x does not belong to the J-F set because the 
                // iterative process converges infinitely
                rez = 1;
                break;
            }
            z0 = z1;
        }

        return rez;
    }

    // it displays a J-F set
    void display(double xmin, double ymin, double xmax, double ymax)
    {
        glPushMatrix();
        glLoadIdentity();

        glBegin(GL_POINTS);
        for (double x = xmin; x <= xmax; x += RX_JF)
            for (double y = ymin; y <= ymax; y += RY_JF)
            {
                CComplex z(x, y);
                int r = isIn(z);
                if (r == 0)
                {
                    glVertex3d(x / 2, y / 2, 0);
                }
                else if (r == -1)
                {

                }
                else if (r == 1)
                {

                }
            }
        fprintf(stdout, "STOP\n");
        glEnd();

        glPopMatrix();
    }

private:
    struct SDate {
        CComplex c;
        // number of iterations
        int nriter;
        // the maximum value of M
        double modmax;
    } m;
};

class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};

class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}

    CPunct& operator=(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    void getxy(double& x, double& y)
    {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector& operator=(CVector& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    // it is used for normalizing a vector
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};

class CSquares
{
public:
    void segmentSquares(double lungime, int nivel, CPunct& p, CVector v)
    {
        CPunct p1;
        if (nivel == 0)
        {
            //v.deseneaza(p, lungime);
        }
        else
        {
            double x, y;
            p.getxy(x, y);

            CPunct p (x + -1 * lungime, y + -1 * lungime);
            segmentSquares(lungime / 3.0, nivel - 1, p, v);

            CPunct p2(x + -1 * lungime, y + 1 * lungime);
            segmentSquares(lungime / 3.0, nivel - 1, p2, v);

            CPunct p3(x + 1 * lungime, y + -1 * lungime);
            segmentSquares(lungime / 3.0, nivel - 1, p3, v);

            CPunct p4(x + 1 * lungime, y + 1 * lungime);
            segmentSquares(lungime / 3.0, nivel - 1, p4, v);

            CPunct p5(x + -1 * lungime, y);
            segmentSquares(lungime / 3.0, nivel - 1, p5, v);

            CPunct p6(x + 1 * lungime, y);
            segmentSquares(lungime / 3.0, nivel - 1, p6, v);

            CPunct p7(x , y + 1 * lungime);
            segmentSquares(lungime / 3.0, nivel - 1, p7, v);

            CPunct p8(x , y + -1 * lungime);
            segmentSquares(lungime / 3.0, nivel - 1, p8, v);

            CPunct p9(x + lungime / 2, y + lungime / 2);
            deseneaza_patrat(lungime,v,p9);

        }
    }

    void deseneaza_patrat(double lungime, CVector v, CPunct p) {
        int rotatie_index = 90;
        while (rotatie_index <= 360) {
            v.rotatie(90);
            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);
            rotatie_index += 90;
        }
    }

    void afisare(double lungime, int nivel)
    {
        printf("am afisat %f,%d", lungime, nivel);
        CVector v1(0.0, 1);
        CPunct p1(0.0, 0.0);

        segmentSquares(lungime, nivel, p1, v1);

        lungime *= 3;
        p1 = CPunct(lungime / 2, lungime / 2);

        v1.rotatie(90);
        v1.deseneaza(p1, lungime);
        p1 = v1.getDest(p1, lungime);


        v1.rotatie(90);
        v1.deseneaza(p1, lungime);
        p1 = v1.getDest(p1, lungime);


        v1.rotatie(90);
        v1.deseneaza(p1, lungime);
        p1 = v1.getDest(p1, lungime);


        v1.rotatie(90);
        v1.deseneaza(p1, lungime);
        p1 = v1.getDest(p1, lungime);



    }

};

class CArbore
{
public:
    void arbore(double lungime,int nivel,double factordiviziune,CPunct p,CVector v)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (nivel == 0)
        {
            return;
        }
        else
        {
            v.rotatie(-45);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            arbore(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            v.rotatie(+90);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            p2 = p1;

            v.rotatie(15);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arbore(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(-60);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            p2 = p1;

            v.rotatie(30);
            v.deseneaza(p1, lungime/2);
            p1 = v.getDest(p1, lungime/2);
            arbore(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(-120);
            v.deseneaza(p1, lungime/2);
            p1 = v.getDest(p1, lungime/2);
            arbore(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 1.0);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        arbore(lungime, nivel, 0.4, p, v);
    }
};


class CTriangle
{
public:
    void deseneaza_3_laturi(double lungime, int nivel, CPunct& p, CVector& v, int d) {
        
        v.rotatie(d * 60);
        v.deseneaza(p, lungime);
        p = v.getDest(p, lungime);
        v.rotatie(-d * 60);
        v.deseneaza(p, lungime);
        p = v.getDest(p, lungime);
        v.rotatie(-d * 60);
        v.deseneaza(p, lungime);
        p = v.getDest(p, lungime);
    }

    void triangle(double lungime, int nivel, CPunct& p, CVector& v, int d)
    {
        if (nivel == 0)
        {
            return;
        }
        else if (nivel == 1) {
            CVector auxiliar_vector = v;
            deseneaza_3_laturi(lungime, nivel, p, v,d);
            v = auxiliar_vector;
            return;
        }
        CVector initial_vector = v;

        v.rotatie(d * 60);
        triangle(lungime/2, nivel - 1, p, v, -d);

        v.rotatie(-d * 60);
        triangle(lungime/2, nivel - 1, p, v, d);


        v.rotatie(-d * 60);
        triangle(lungime/2, nivel - 1, p, v, -d);

        v = initial_vector;

        
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(-0.7, 0.9);

        triangle(lungime, nivel, p, v, 1);
    }
};


void Display1() {
    CComplex c(-0.12375, 0.056805);

    CMandelbrot cjf(c);
    glColor3f(1.0, 0.1, 0.1);
    cjf.setnriter(10);
    cjf.display(-2, -2, 2, 2);
}

void Display2() {
    CSquares cck;
    cck.afisare(0.5, nivel+1);
    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);


    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 's');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'q');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 's');


    nivel++;
}

void Display3() {
    CArbore cab;
    cab.afisare(0.3, nivel);
    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');

    nivel++;
}

void Display4() {
    CTriangle cbh;
    cbh.afisare(0.8, nivel);
    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    nivel++;

}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    switch (prevKey)
    {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        nivel = 0;
        fprintf(stderr, "nivel = %d\n", nivel);
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display1();
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        Display2();
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        Display3();
        break;
    case '4':
        glClear(GL_COLOR_BUFFER_BIT);
        Display4();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

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