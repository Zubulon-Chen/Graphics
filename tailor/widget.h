#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QtGlobal>
#include <QTime>
#include <QVector>
#include <QDebug>
#include <QMouseEvent>


namespace Ui {
class Widget;
}

enum edges{LEFT,RIGHT,UP,DOWN};
enum choice{LINE,POLYGEN};
enum type{INPUT,RANDOM};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void LB_lineClip(int x1, int y1, int x2, int y2, QPainter *painter);
    bool clip(int p,int q,double &u1,double &u2);
    void line_clip(QPainter &painter);
    QVector<QPoint> SH_clip(const QVector<QPoint> &QPoints);
    QVector<QPoint> one_edge_clip(const QVector<QPoint> &points, edges e);
    bool inside(QPoint &p, edges e);
    QPoint getIntersect(QPoint a, QPoint b, bool ina, edges e);
    void polygen_clip(QPainter &painter);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    const int wid;
    const int ht;
    int xl,xr,yb,yt;
    choice cc;
    type tp;
    QVector<QPoint> polygenPoints;
    QVector<QPoint> linePoints;
    bool inputOver;
};

#endif // WIDGET_H
