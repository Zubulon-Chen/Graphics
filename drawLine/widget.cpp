#include "widget.h"
#include "ui_widget.h"
const int numLines=80;
const int gap=10;
const int lineLength=numLines*gap;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(lineLength,lineLength);
}

void Widget::DDA(QPainter *painter,int x1,int y1,int x2,int y2,const QColor &color)
{
    double dx,dy,k,y;
    dx=x2-x1;
    dy=y2-y1;
    k=dy/dx;
    y=y1;
    for(int x=x1;x<=x2;x++){
        painter->fillRect(x*gap,round(y)*gap,gap,gap,color);
        y+=k;
    }
}

void Widget::midPoint(QPainter *painter, int x1, int y1, int x2, int y2, const QColor &color)
{
    int a,b,d1,d2,d,x,y;
    a=y1-y2;
    b=x2-x1;
    d=2*a+b;
    d1=2*a;
    d2=2*(a+b);
    x=x1;
    y=y1;
    painter->fillRect(x*gap,y*gap,gap,gap,color);
    while(x<x2){
        if(d<0){
            x++;y++;d+=d2;
        }
        else{
            x++;d+=d1;
        }
        painter->fillRect(x*gap,y*gap,gap,gap,color);
    }
}

void Widget::bresenham(QPainter *painter, int x1, int y1, int x2, int y2, const QColor &color)
{
    int x,y,dx,dy;
    double k,e;
    dx=x2-x1;
    dy=y2-y1;
    k=dy/dx;
    e=-0.5;
    x=x1;
    y=y1;
    for(int i=0;i<=dx;i++){
        painter->fillRect(x*gap,y*gap,gap,gap,color);
        x++;
        e+=k;
        if(e>=0){
            y++;
            e--;
        }
    }
}

void Widget::circlePoints(QPainter *painter,int x, int y, const QColor &color)
{
    painter->fillRect(x*gap,y*gap,gap,gap,color);
    painter->fillRect(y*gap,x*gap,gap,gap,color);
    painter->fillRect(-x*gap,y*gap,gap,gap,color);
    painter->fillRect(y*gap,-x*gap,gap,gap,color);
    painter->fillRect(x*gap,-y*gap,gap,gap,color);
    painter->fillRect(-y*gap,x*gap,gap,gap,color);
    painter->fillRect(-x*gap,-y*gap,gap,gap,color);
    painter->fillRect(-y*gap,-x*gap,gap,gap,color);
}

void Widget::midPointCircle(QPainter *painter,int x0,int y0, int r, const QColor &color)
{
    painter->save();
    painter->translate(x0,y0);
    int x=0,y=r;
    double d=1.25-r;
    circlePoints(painter,x,y,color);
    while(x<=y){
        if(d<0)
            d+=2*x+3;
        else{
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        circlePoints(painter,x,y,color);
    }
    painter->restore();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    for(int i=0;i<numLines;i++){
        painter.drawLine(gap*i,0,gap*i,lineLength);
    }
    for(int i=0;i<numLines;i++){
        painter.drawLine(0,gap*i,lineLength,gap*i);
    }
//    painter.translate(200,200);
    DDA(&painter,30,40,20,25,Qt::red);
    midPoint(&painter,7,80,30,35,Qt::blue);
    bresenham(&painter,23,23,33,33,Qt::yellow);
    midPointCircle(&painter,100,100,10,Qt::red);
}
