#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void DDA(QPainter *painter, int x1, int y1, int x2, int y2, const QColor &color);
    void midPoint(QPainter *painter, int x1, int y1, int x2, int y2, const QColor &color);
    void bresenham(QPainter *painter, int x1, int y1, int x2, int y2, const QColor &color);
    void circlePoints(QPainter *painter, int x, int y, const QColor &color);
    void midPointCircle(QPainter *painter, int x0, int y0, int r, const QColor &color);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
