#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QPainter>
#include <algorithm>
#include <QMouseEvent>

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
    bool operator <(const edge& a) const{
        if(xmin!=a.xmin)
            return xmin<a.xmin;
        return dx<a.dx;
    }
};

struct point
{
    int x,y;
    point() {}
    point(int xx,int yy):x(xx),y(yy) {}
};

enum state{INPUT,OVER};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent=0);
    ~Widget();
    void polygonScan(QPainter &painter);
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void on_input_Button_clicked();
    void mousePressEvent(QMouseEvent* event);

    void on_over_Button_clicked();

private:
    Ui::Widget *ui;
    QVector<QList<edge>> ET;//边表，记录所有边的信息
    QList<edge> AET;//活性边表，与当前扫描线相交的边
    QVector<point> vertices;//多边形的顶点集合
    state ss;
};

#endif // WIDGET_H
