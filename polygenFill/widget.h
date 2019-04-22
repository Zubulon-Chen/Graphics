#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QPainter>
#include <algorithm>
using namespace std;
namespace Ui {
class Widget;
}
struct edge
{
    int ymax;//上端点的y值
    double xmin;//左端点的x值
    double dx;//斜率的倒数，y的一个增量对应的x的增量
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
    QVector<QList<edge>> ET;//边表，记录所有边的信息
    QList<edge> AET;//活性边表，与当前扫描线相交的边
    int winHeight,winWidth;
    QVector<point> vertices;//多边形的顶点集合
};

#endif // WIDGET_H
