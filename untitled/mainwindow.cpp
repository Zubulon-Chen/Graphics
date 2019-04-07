#include "mainwindow.h"

MyWidget::MyWidget() {
    img = new QImage(800, 600, QImage::Format_RGB32);
    FillBackground();
    cpyImg = img -> copy();
    this -> setMouseTracking(true);
    pressed = false;
    shiftPressed = false;
    firstCurve = true;
    currentPoint = 0;
    i = 0;
}

MyWidget::~MyWidget() {
    delete img;
    img = NULL;
}

double MyWidget::Power(double x, int n) {
    double temp = x;

    for(int i = 1; i < n; i++) {
        x = x * temp;
    }

    return x;
}

double MyWidget::distanceBetween(int x1, int y1, int x2, int y2) {
    double o = sqrt( ( (x2 - x1) * (x2 - x1) ) + ( (y2 - y1) * (y2 - y1) )  );
    return o;
}

void MyWidget::FillBackground() {
    int i,j;
    unsigned char *ptr;
    ptr = img -> bits();

    for (j = 0; j < img -> height(); j++) {
        for (i = 0;i < img -> width(); i++) {
            ptr[(img -> width() * j + i) * 4] = 100;
            ptr[(img -> width() * j + i) * 4 + 1] = 100;
            ptr[(img -> width() * j + i) * 4 + 2] = 100;
        }
    }
}

void MyWidget::DrawLine(double x1, double y1, double x2, double y2) {
    int y, x;
    int tempX;
    int tempY;

    // Wspolczynniki prostej
    a = (y2 - y1) / (x2 - x1);
    b = y1 - ( ( (y2 - y1) / (x2 - x1) ) * x1 );

    // Pionowe linie
    if(x1 == x2) {
        if(y1 > y2) {
            tempY = y1;
            y1 = y2;
            y2 = tempY;
        }
        for(int i = y1; i < y2; i++) {
            SetPixel(x1, i);
        }
    }

    // Rysowanie dla a w danym przedziale (po iksach)
    else if(a <= 1 && a >= -1) {
        if(x1 > x2) {
            tempX = x1;
            x1 = x2;
            x2 = tempX;

            tempY = y1;
            y1 = y2;
            y2 = tempY;
        }

        for(int i = x1; i < x2; i++) {
            y = (a * i) + b;
            SetPixel(i, y);
        }
    }

    // Rysowanie dla a w danym przedziale (po igerkach)
    else {
        if(y1 > y2) {
            tempX = x1;
            x1 = x2;
            x2 = tempX;

            tempY = y1;
            y1 = y2;
            y2 = tempY;
        }

        for(int i = y1; i < y2; i++) {
            x = (i - b) / a;
            SetPixel(x, i);
        }
    }
}

void MyWidget::DrawRedLine(double x1, double y1, double x2, double y2) {
    int y, x;
    int tempX;
    int tempY;

    // Wspolczynniki prostej
    a = (y2 - y1) / (x2 - x1);
    b = y1 - ( ( (y2 - y1) / (x2 - x1) ) * x1 );

    // Pionowe linie
    if(x1 == x2) {
        if(y1 > y2) {
            tempY = y1;
            y1 = y2;
            y2 = tempY;
        }
        for(int i = y1; i < y2; i++) {
            SetRedPixel(x1, i);
        }
    }

    // Rysowanie dla a w danym przedziale (po iksach)
    else if(a <= 1 && a >= -1) {
        if(x1 > x2) {
            tempX = x1;
            x1 = x2;
            x2 = tempX;

            tempY = y1;
            y1 = y2;
            y2 = tempY;
        }

        for(int i = x1; i < x2; i++) {
            y = (a * i) + b;
            SetRedPixel(i, y);
        }
    }

    // Rysowanie dla a w danym przedziale (po igerkach)
    else {
        if(y1 > y2) {
            tempX = x1;
            x1 = x2;
            x2 = tempX;

            tempY = y1;
            y1 = y2;
            y2 = tempY;
        }

        for(int i = y1; i < y2; i++) {
            x = (i - b) / a;
            SetRedPixel(x, i);
        }
    }
}

void MyWidget::DrawCircle(double r, int x, int y) {
    double Y;

    for(int i = 0; i <= r / sqrt(2); i++) {
        Y = sqrt( (r * r) - (i * i) );

        SetPixel(i + x, Y + y);
        SetPixel(i + x, -Y + y);
        SetPixel(-i + x, Y + y);
        SetPixel(-i +x, -Y + y);

        SetPixel(Y + x, i + y);
        SetPixel(Y + x, -i + y);
        SetPixel(-Y + x, i + y);
        SetPixel(-Y + x, -i + y);
    }
}

void MyWidget::DrawBeziersCurve(BCurve* b) {
    DrawCircle(7, b -> p0x, b -> p0y);
    DrawCircle(7, b -> p1x, b -> p1y);
    DrawCircle(7, b -> p2x, b -> p2y);
    DrawCircle(7, b -> p3x, b -> p3y);

    SetPixel(b -> p0x, b -> p0y);
    SetPixel(b -> p1x, b -> p1y);
    SetPixel(b -> p2x, b -> p2y);
    SetPixel(b -> p3x, b -> p3y);

    DrawRedLine(b -> p0x, b -> p0y, b -> p1x, b -> p1y);
    DrawRedLine(b -> p3x, b -> p3y, b -> p2x, b -> p2y);

    double Px, Py, PtempX, PtempY;

    PtempX = b -> p0x;
    PtempY = b -> p0y;

    for(double i = 0.01; i <= 1; i = i + 0.01) {
        Px = ( (Power( (1 - i), 3  ) ) * b -> p0x ) + ( 3 * ( Power( (1 - i), 2  ) ) * i * b -> p1x ) + (3 * (1 - i) * (Power(i, 2) ) * b -> p2x ) + ( (Power(i, 3) ) * b -> p3x);
        Py = ( (Power( (1 - i), 3  ) ) * b -> p0y ) + ( 3 * ( Power( (1 - i), 2  ) ) * i * b -> p1y ) + (3 * (1 - i) * (Power(i, 2) ) * b -> p2y ) + ( (Power(i, 3) ) * b -> p3y);

        DrawLine(PtempX, PtempY, Px, Py);

        PtempX = Px;
        PtempY = Py;

    }
}

void MyWidget::DrawAllCurves() {
    for(int j = 0; j < i; j++) {
        DrawBeziersCurve(curves[j]);
    }
}

void MyWidget::SetPixel(int y, int x) {
    unsigned char *ptr;

    // Najpierw sprawdza czy nie wyszedlem za ekran
    if(y > 0 && y < width() && x > 0 && x < height() ) {
        // Rysuj po kopii
        if(pressed) {
            ptr = cpyImg.bits();
            ptr[(cpyImg.width() * x + y) * 4] = 255;
            ptr[(cpyImg.width() * x + y) * 4 + 1] = 255;
            ptr[(cpyImg.width() * x + y) * 4 + 2] = 255;
        }

        // Rysuj po oryginale
        else {
            ptr = img -> bits();
            ptr[(img -> width() * x + y) * 4] = 255;
            ptr[(img -> width() * x + y) * 4 + 1] = 255;
            ptr[(img -> width() * x + y) * 4 + 2] = 255;
        }
    }
}

void MyWidget::SetRedPixel(int y, int x) {
    unsigned char *ptr;

    // Najpierw sprawdza czy nie wyszedlem za ekran
    if(y > 0 && y < width() && x > 0 && x < height() ) {
        // Rysuj po kopii
        if(pressed) {
            ptr = cpyImg.bits();
            ptr[(cpyImg.width() * x + y) * 4] = 0;
            ptr[(cpyImg.width() * x + y) * 4 + 1] = 0;
            ptr[(cpyImg.width() * x + y) * 4 + 2] = 255;
        }

        // Rysuj po oryginale
        else {
            ptr = img -> bits();
            ptr[(img -> width() * x + y) * 4] = 0;
            ptr[(img -> width() * x + y) * 4 + 1] = 0;
            ptr[(img -> width() * x + y) * 4 + 2] = 255;
        }
    }
}

void MyWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);

    if(pressed) {
        p.drawImage(0, 0, cpyImg);
    }
    else {
        p.drawImage(0, 0, *img);
    }
}

bool MyWidget::isInsideCircle(int x, int y, double r) {
    int o = distanceBetween(x, y, X1, Y1);

    if(o < r) return true;
    else return false;
}

bool MyWidget::isInAnyCircle() {
    for(int j = 0; j < i; j++) {
        if(curves[j] -> isInsideC0) return true;
        if(curves[j] -> isInsideC1) return true;
        if(curves[j] -> isInsideC2) return true;
        if(curves[j] -> isInsideC3) return true;
    }

    return false;
}

void MyWidget::keyPressEvent(QKeyEvent* event) {
    if(event -> key() == Qt::Key_Shift) {
        shiftPressed = true;
    }
}

void MyWidget::keyReleaseEvent(QKeyEvent* event) {
    if(event -> key() == Qt::Key_Shift) {
        shiftPressed = false;
    }
}


void MyWidget::mouseReleaseEvent(QMouseEvent* event) {
    pressed = false;

    if (event -> button() == Qt::RightButton) {
        FillBackground();
        repaint();
        return;
    }



    // Pobieram punkt w ktorym puscilem kursor
    QPoint P = event -> pos();
    X2 = P.x();
    Y2 = P.y();

    // Jesli wlasnie puscilem przeciaganie jakiegos kolka
    if(isInAnyCircle() ) {
        FillBackground();
        DrawAllCurves();
    }

    // Jesli kliknalem w obszar bez kolka
    else {
        // Tworzenie pierwszej krzywej
        if(firstCurve) {
            switch(currentPoint) {
            case 0:
                i++;
                curves.push_back(new BCurve);

                // Pierwszy punkt krzywej
                curves[i - 1] -> setP0(P.x(), P.y() );

                // Dwa pozostale punkty najpierw ustawiam w miejsce poczatkowego
                curves[i - 1] -> setP3(P.x(), P.y() );
                curves[i - 1] -> setP2(P.x(), P.y() );

                // Pierwszy punkt kontrolny
                curves[i - 1] -> setP1(P.x(), P.y() );

                SetPixel(X2, Y2);
                DrawCircle(7, X2, Y2);

                currentPoint++;

                break;

            case 1:
                // Ostatni punkt krzywej
                curves[i - 1] -> setP3(P.x(), P.y() );

                // Drugi punkt kontrolny
                curves[i - 1] -> setP2(P.x(), P.y() );


                SetPixel(X2, Y2);
                DrawCircle(7, X2, Y2);
                DrawLine(curves[i - 1] -> p0x, curves[i - 1] -> p0y, X2, Y2);

                currentPoint = 0;
                firstCurve = false;

                break;            
            }
        }
        // Tworzenie pozostalych krzywych
        // W tym przypadku znam dwa punkty nowej krzywej, pierwszy ktory jest ostatnim poprzedniej oraz jeden z punktow kontrolnych
        else {
            switch(currentPoint) {
            case 0:
                i++;

                curves.push_back(new BCurve);

                // Ustawianie poczatkowego punktu ostatnim poprzedniej krzywej
                curves[i - 1] -> setP0(curves[i - 2] -> p3x, curves[i - 2] -> p3y);
                curves[i - 1] -> setP3(P.x(), P.y() );

                // Drugi punkt kontrolny
                curves[i - 1] -> setP2(P.x(), P.y() );

                // Pierwszy punkt kontrolny
                curves[i - 1] -> setP1(curves[i - 2] -> p3x, curves[i - 2] -> p3y);

                // Punkt konca krzywej
                SetPixel(X2, Y2);
                DrawCircle(7, X2, Y2);
                DrawLine(curves[i - 1] -> p0x, curves[i - 1] -> p0y, X2, Y2);

                /*
                // Rysowanie symetrycznego punktu kontrolnego
                X2 = curvy[i - 1] -> p0x + (curvy[i - 1] -> p0x - curvy[i - 2] -> p2x);
                Y2 = curvy[i - 1] -> p0y + (curvy[i - 1] -> p0y - curvy[i - 2] -> p2y);

                curvy[i - 1] -> setP1(X2, Y2);

                SetPixel(X2, Y2);
                DrawCircle(7, X2, Y2);
                DrawRedLine(curvy[i - 1] -> p0x, curvy[i - 1] -> p0y, X2, Y2);
                */
                currentPoint = 0;

                break;
            }
        }
    }

    // Po odkliknieciu na pewno nie klikam zadnego kolka, wiec ustaw wszystkie sprawdzenia na falsz
    for(int j = 0; j < i; j++) {
        curves[j] -> isInsideC0 = false;
        curves[j] -> isInsideC1 = false;
        curves[j] -> isInsideC2 = false;
        curves[j] -> isInsideC3 = false;
    }

    repaint();
}

void MyWidget:: mousePressEvent(QMouseEvent* event) {
    pressed = true;

    if (event -> button() == Qt::LeftButton) {
        QPoint P = event -> pos();

        X1 = P.x();
        Y1 = P.y();

      // Sprawdzam czy kliknalem w jakies kolko, jesli tak, to ustaw dla danej krzywej, ze kursor znajduje sie w danym kolku
        for(int j = 0; j < i; j++) {
            if(isInsideCircle(curves[j] -> p1x, curves[j] -> p1y, 7) ) {
                curves[j] -> isInsideC1 = true;
                // Naprawa bug'a z rozdzielaniem sie konstrukcji
                if(j != 0) curves[j - 1] -> isInsideC3 = false;
            }
            else if(isInsideCircle(curves[j] -> p0x, curves[j] -> p0y, 7) ) curves[j] -> isInsideC0 = true;
            else if(isInsideCircle(curves[j] -> p2x, curves[j] -> p2y, 7) ) curves[j] -> isInsideC2 = true;
            else if(isInsideCircle(curves[j] -> p3x, curves[j] -> p3y, 7) ) curves[j] -> isInsideC3 = true;
        }

    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if(pressed) {
        cpyImg = img -> copy();
        QPoint P = event->pos();
        X2 = P.x();
        Y2 = P.y();

        // Sprawdzam czy jestem w jakims kolku, jesli tak, to zmien konkretne punkty
        for(int j = 0; j < i; j++) {
            if(curves[j] -> isInsideC0) {
                curves[j] -> setP0(X2, Y2);
                FillBackground();
                DrawAllCurves();
            }

            else if(curves[j] -> isInsideC1) {
                curves[j] -> setP1(X2, Y2);
                // Jesli nie kliknalem na pierwszy punkt calej konstrukcji i nie trzymam klawisza shift
                if(j != 0 && !shiftPressed) {
                    // Zrob symetryczny punkt kontrolny
                    X2 = curves[j] -> p0x + (curves[j] -> p0x - X2);
                    Y2 = curves[j] -> p0y + (curves[j] -> p0y - Y2);
                    curves[j - 1] -> setP2(X2, Y2);
                }

                FillBackground();
                DrawAllCurves();
            }

            else if(curves[j] -> isInsideC2) {
                curves[j] -> setP2(X2, Y2);
                // Jesli nie kliknalem na ostatni punkt calej konstrukcji i nie trzymam klawisza shift
                if(j != i - 1 && !shiftPressed) {
                    // Zrob symetryczny punkt kontrolny
                    X2 = curves[j] -> p3x + (curves[j] -> p3x - X2);
                    Y2 = curves[j] -> p3y + (curves[j] -> p3y - Y2);
                    curves[j + 1] -> setP1( X2, Y2 );
                }


                FillBackground();
                DrawAllCurves();
            }

            else if(curves[j] -> isInsideC3) {
                curves[j] -> setP3(X2, Y2);
                FillBackground();
                DrawAllCurves();
            }
        }

        repaint();
    }
}
