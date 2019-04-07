#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(1000,800);
    points={QPoint(100,300),QPoint(250,100),QPoint(400,100),QPoint(600,500)};
    idx=0;
    change=false;
    m=0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::drawLines(const QVector<QPoint> &points,QPainter &painter)
{
    for(int i=0;i<points.size()-1;i++)
        painter.drawLine(points[i],points[i+1]);
}

void Widget::drawBezier(QPainter &painter)
{
    QVector<QPoint> ppp=points;
    drawLines(points,painter);
    QVector<QPoint> pp;
    for(double t=0.0;t<=1.0;t+=0.001){
        points=ppp;
        while(points.size()>1){
            for(int i=0;i<points.size()-1;i++){
                int xx=points[i].x()*(1-t)+points[i+1].x()*t;
                int yy=points[i].y()*(1-t)+points[i+1].y()*t;
                pp.push_back(QPoint(xx,yy));
            }
            points=pp;
            pp.clear();
        }
        painter.drawPoint(points.front());
    }
    points=ppp;
}



void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    drawBezier(painter);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        for(int i=0;i<points.size();i++){
            if(abs(event->pos().x()-points[i].x())<=5
                    &&abs(event->pos().y()-points[i].y())<=5){

                idx=i;

                change=1;
                setCursor(Qt::CrossCursor);
                break;
            }
        }
    }else {

        setCursor(Qt::ArrowCursor);

    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    m=0;  //还应该在这在给m 赋一次值 为了清空鼠标上一次在某个点上留下的值

    QString text=QString("%1 %2").arg(event->pos().x()).arg(event->pos().y());

    QToolTip::showText(QPoint(event->globalPos()),text,this);  //显示鼠标坐标  更随鼠标用方框的形式

    //在mainwindows 这个类中用 mouseMoveEevent 事件只有点击拖动鼠标时才会有效 不知道是什么原因

    for(int i=0;i<points.size();i++){

        if(fabs(event->pos().x()-points[i].x())<=5&& fabs(event->pos().y()-points[i].y())<=5){

            m=m+1;

        }}

    if(m!=0) {
        setCursor(Qt::CrossCursor);  //改变鼠标的样式
        update();  //刷新
    }else {
        setCursor(Qt::ArrowCursor);
        update();
    }
    if(change==1){  //改变点的位置
        points[idx]=QPoint(event->pos().x(),event->pos().y());
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    update();
}
