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
#include <algorithm>

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
    void drawB_Spline(QPainter &painter);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
    int numPoints;//记录输出顶点个数
    QVector<QPoint> points;
    int idx;//选中的是哪个顶点
    bool change;//顶点是否变动
    bool click_points;//是否还在添加顶点
    bool delete_point;//是否在删除顶点
    bool isBezier;//判断当前是bezier曲线还是B样条曲线
    int degree;//B样条曲线的阶数，次数p=degree-1
};

#endif // WIDGET_H
