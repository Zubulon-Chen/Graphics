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
#include <QInputDialog>
#include <QDebug>

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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    int numPoints;
    QVector<QPoint> points;
    int idx;
    bool change;
    int m,m_mouse_style,j;
    bool click_points;
    bool delete_point;
};

#endif // WIDGET_H
