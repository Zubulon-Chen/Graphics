#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QEventLoop>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QToolTip>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void drawLines(const QVector<QPoint> &points, QPainter &painter);
    void drawBezier(QPainter &painter);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Widget *ui;
    QVector<QPoint> points;
    int idx;
    bool change;
    int m,m_mouse_style,j;
};

#endif // WIDGET_H
