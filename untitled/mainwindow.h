#include <QWidget>
#include <QImage>
#include <QRgb>
#include <QPainter>
#include <QLabel>
#include <iostream>
#include <QPoint>
#include <QMouseEvent>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include "bcurve.h"
using namespace std;

class MyWidget : public QWidget {
    Q_OBJECT

    QImage *img;
    QImage cpyImg;

    public:
        MyWidget();
        ~MyWidget();

        // Potegowanie liczby do potegi n
        double Power(double x, int n);

        // Odleglosc pomiedzy dwoma punktami
        double distanceBetween(int x1, int y1, int x2, int y2);

        // Czysci obraz
        void FillBackground();

        // Rysowanie zwyklej lini i czerwonej
        void DrawLine(double x1, double y1, double x2, double y2);
        void DrawRedLine(double x1, double y1, double x2, double y2);

        // Rysowanie kola
        void DrawCircle(double r, int x, int y);

        // Rysowanie pojedyncze krzywej oraz wszystkich istniejacych
        void DrawBeziersCurve(BCurve* b);
        void DrawAllCurves();

        // Sprawdzenia czy punkt jest wewnatrz kola i czy jaki kolwiek punkt jest wewnatrz kola
        bool isInsideCircle(int x, int y, double r);
        bool isInAnyCircle();

    protected:
        vector<BCurve*> curves;
        int currentPoint, i;
        bool pressed, firstCurve, shiftPressed;
        double X1, Y1, X2, Y2, a, b;

        void SetPixel(int x, int y);
        void SetRedPixel(int x, int y);
        void paintEvent(QPaintEvent*);
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
};
