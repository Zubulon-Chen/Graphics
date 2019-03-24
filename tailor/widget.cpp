#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    wid(1200),ht(900)
{
    ui->setupUi(this);
    setFixedSize(wid,ht);
    xl=wid/3;
    xr=2*xl;
    yb=ht/3;
    yt=yb*2;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::LB_lineClip(int x1, int y1, int x2, int y2,QPainter *painter)
{

    int dx=x2-x1,dy=y2-y1;
    double u1=0.0,u2=1.0;
    if(clip(-dx,x1-xl,u1,u2)){
        if(clip(dx,xr-x1,u1,u2)){
            if(clip(-dy,y1-yb,u1,u2)){
                if(clip(dy,yt-y1,u1,u2)){
                    QPen pen1(Qt::gray, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
                    painter->setPen(pen1);
                    painter->drawLine(x1,y1,x1+round(u1*dx),y1+round(u1*dy));
                    painter->drawLine(x1+round(u2*dx),y1+round(u2*dy),x2,y2);
                    painter->setPen(QPen(Qt::darkGreen, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                    painter->drawLine(x1+round(u1*dx),y1+round(u1*dy),x1+round(u2*dx),y1+round(u2*dy));

                }
            }
        }
    }
}

bool Widget::clip(int p, int q, double &u1, double &u2)
{
    double r=(q*1.0)/(p*1.0);
    if(p<0){
        if(r>u2)
            return false;
        if(r>u1)
            u1=r;
    }
    else if(p>0){
        if(r<u1)
            return false;
        if(r<u2)
            u2=r;
    }
    else
        return q>=0;
    return true;
}

void Widget::line_clip_test(QPainter &painter)
{
    //生成一些随机线段，测试LB算法
    painter.setRenderHint(QPainter::Antialiasing, true);
    const int numLines=80;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<numLines; i++) {
        LB_lineClip(qrand()%wid,qrand()%wid,qrand()%ht,qrand()%ht,&painter);
    }
}

QVector<QPoint> Widget::SH_clip(const QVector<QPoint> &QPoints)
{
    QVector<QPoint> ans;
    ans=one_edge_clip(QPoints,LEFT);
    ans=one_edge_clip(ans,UP);
    ans=one_edge_clip(ans,RIGHT);

    ans=one_edge_clip(ans,DOWN);
    return ans;
}

QVector<QPoint> Widget::one_edge_clip(const QVector<QPoint> &points, edges e)
{
    QVector<QPoint> ans;
    QPoint pp;
    QPoint prev=points.back();
    for(auto p:points){
        if(inside(p,e)){
            if(inside(prev,e))
                ans.push_back(p);
            else{
                pp=getIntersect(prev,p,false,e);
                ans.push_back(pp);
                ans.push_back(p);
            }
        }
        else if(inside(prev,e)){
            pp=getIntersect(prev,p,true,e);
            ans.push_back(pp);
        }
        prev=p;
    }
    return ans;
}

bool Widget::inside(QPoint &p, edges e)
{
    switch (e) {
    case LEFT:
        return p.x()>=xl;
    case RIGHT:
        return p.x()<=xr;
    case UP:
        return p.y()<=yt;
    case DOWN:
        return p.y()>=yb;
    }
}

//获得ab连线与窗口的交点，保证一个在内一个在外
QPoint Widget::getIntersect(QPoint a, QPoint b,bool ina, edges e)
{
    //使得a在窗口内
    if(!ina){
        QPoint tmp;
        tmp=a;
        a=b;
        b=tmp;
    }
    double k=(double)((b.y()-a.y())*1.0)/((b.x()-a.x())*1.0);
    switch (e) {
    case LEFT:
        return QPoint(xl,round(b.y()+(xl-b.x())*k));
    case RIGHT:
        return QPoint(xr,round(a.y()+(xr-a.x())*k));
    case UP:
        return QPoint(round(a.x()+(yt-a.y())/k),yt);
    case DOWN:
        return QPoint(round(b.x()+(yb-b.y())/k),yb);
    }
}

void Widget::polygen_clip_test(QPainter &painter)
{
    //生成随机点，构成多边形
    QVector<QPoint> vertices = { QPoint(350, 450), QPoint(600, 150), QPoint(700, 400),
                                 QPoint(750, 550), QPoint(650, 800), QPoint(500, 800)
                               };
    //将原图形以灰色虚线画出
    painter.setPen(QPen(Qt::gray, 5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    for(int i=0;i<vertices.size();i++){
        if(i==0){
            painter.drawLine(vertices.back(),vertices[i]);
        }
        else{
            painter.drawLine(vertices[i-1],vertices[i]);
        }
    }
    QVector<QPoint> ans=SH_clip(vertices);
    painter.setPen(QPen(Qt::darkGreen, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for(int i=0;i<ans.size();i++){
        if(i==0){
            painter.drawLine(ans.back(),ans[i]);
        }
        else{
            painter.drawLine(ans[i-1],ans[i]);
        }
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(6);
    painter.setPen(pen);
    //绘制九宫格
    int ww=wid/3;
    int hh=ht/3;
    painter.drawLine(ww,0,ww,ht);
    painter.drawLine(2*ww,0,2*ww,ht);
    painter.drawLine(0,hh,wid,hh);
    painter.drawLine(0,2*hh,wid,2*hh);

    //线段裁剪测试
    //    line_clip_test(painter);
    //多边形裁剪测试
    polygen_clip_test(painter);

}
