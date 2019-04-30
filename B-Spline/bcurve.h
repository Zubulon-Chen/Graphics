#ifndef BCURVE_H
#define BCURVE_H
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
using namespace std;

class MyWidget;
class BCurve {
friend class MyWidget;
private:
    int p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y;

    // Zmienne boolowskie ktore mowia czy kursor znajduje sie w ktoryms kolku punktow krzywej
    bool isInsideC0, isInsideC1, isInsideC2, isInsideC3;

public:
    BCurve();

    void setP0(int p0x, int p0y);
    void setP1(int p1x, int p1y);
    void setP2(int p2x, int p2y);
    void setP3(int p3x, int p3y);
};

#endif // BCURVE_H
