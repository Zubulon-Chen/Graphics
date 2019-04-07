#include "bcurve.h"

BCurve::BCurve() {
    p0x = -1;
    p0y = -1;
    p1x = -1;
    p1y = -1;
    p2x = -1;
    p2y = -1;
    p3x = -1;
    p3y = -1;

    isInsideC0 = false;
    isInsideC1 = false;
    isInsideC2 = false;
    isInsideC3 = false;
}

void BCurve::setP0(int p0x, int p0y) {
    this -> p0x = p0x;
    this -> p0y = p0y;
}

void BCurve::setP1(int p1x, int p1y) {
    this -> p1x = p1x;
    this -> p1y = p1y;
}

void BCurve::setP2(int p2x, int p2y) {
    this -> p2x = p2x;
    this -> p2y = p2y;
}

void BCurve::setP3(int p3x, int p3y) {
    this -> p3x = p3x;
    this -> p3y = p3y;
}
