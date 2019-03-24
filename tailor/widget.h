#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QtGlobal>
#include <QTime>
#include <QVector>

namespace Ui {
class Widget;
}
//struct QPoint
//{
//    int x,y;
//    QPoint() {}
//    QPoint(int xx,int yy):x(xx),y(yy) {}
//};
//struct edge
//{
//    int x1,y1,x2,y2;
//    edge() {}
//};

enum edges{LEFT,RIGHT,UP,DOWN};
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void LB_lineClip(int x1, int y1, int x2, int y2, QPainter *painter);
    bool clip(int p,int q,double &u1,double &u2);
    void line_clip_test(QPainter &painter);
    QVector<QPoint> SH_clip(const QVector<QPoint> &QPoints);
    QVector<QPoint> one_edge_clip(const QVector<QPoint> &points, edges e);
    bool inside(QPoint &p, edges e);
    QPoint getIntersect(QPoint a, QPoint b, bool ina, edges e);
    void polygen_clip_test(QPainter &painter);
protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Widget *ui;
    const int wid;
    const int ht;
    int xl,xr,yb,yt;

};

#endif // WIDGET_H
