#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QPainter>
namespace Ui {
class Widget;
}
struct edge
{
    int ymax;
    double xmin;
    double dx;
    edge() {}
    edge(int yy,double xx,double dd):ymax(yy),xmin(xx),dx(dd) {}
};

struct point
{
    int x,y;
    point() {}
    point(int xx,int yy):x(xx),y(yy) {}
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QVector<point> vv,int w,int h);
    ~Widget();
    void polygonScan(QPainter &painter);
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::Widget *ui;
    QVector<QList<edge>> ET;
    QList<edge> AET;
    int winHeight,winWidth;
    QVector<point> vertices;
};

#endif // WIDGET_H
