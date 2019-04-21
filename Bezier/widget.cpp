#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    setFixedSize(1000,800);
    points={QPoint(100,300),QPoint(250,100),QPoint(400,100),QPoint(600,500)};
    idx=0;
    change=false;
    m=0;
    click_points=true;
    delete_point=false;
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
    painter.setPen(QPen(Qt::darkGreen, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    drawLines(points,painter);
    QVector<QPoint> pp;
    painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    qDebug()<<"paint event\n";
    if(!click_points)
        drawBezier(painter);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        qDebug()<<"press event\n";
        if(click_points){
            points.push_back(event->pos());
            if(points.size()==numPoints){

                click_points=false;
                update();
            }
        }
        else if(delete_point){
            for(int i=0;i<points.size();i++){
                if(abs(event->pos().x()-points[i].x())<=5
                        &&abs(event->pos().y()-points[i].y())<=5){
                    setCursor(Qt::CrossCursor);
                    points.remove(i);
                    delete_point=false;
                    update();
                    break;
                }
            }
        }
        else{
            for(int i=0;i<points.size();i++){
                if(abs(event->pos().x()-points[i].x())<=5
                        &&abs(event->pos().y()-points[i].y())<=5){

                    idx=i;

                    change=true;
                    setCursor(Qt::CrossCursor);
                    break;
                }
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
    if(change){  //改变点的位置
        points[idx]=QPoint(event->pos().x(),event->pos().y());
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    update();
}

void Widget::on_pushButton_clicked()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Input points number"),
                                 tr("input: "), 4, 0, 100, 1, &ok);
    if (ok){
        points.clear();
        numPoints=i;
        click_points=true;
    }
}

void Widget::on_pushButton_2_clicked()
{
    delete_point=true;
}
