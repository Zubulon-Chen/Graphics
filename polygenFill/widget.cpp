#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent, QVector<point> vv, int w, int h) :
    QWidget(parent),
    ui(new Ui::Widget),
    vertices(vv),winWidth(w),winHeight(h)
{
    ui->setupUi(this);
    setFixedSize(winWidth,winHeight);
    ET.resize(winHeight);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::polygonScan(QPainter &painter)
{
    int maxy=0,miny=INT_MAX;//记录整个多边形的上下边界
    for(auto it:vertices){
        if(it.y>maxy)
            maxy=it.y;
        if(it.y<miny)
            miny=it.y;
    }
    //遍历顶点集合，建立边表ET
    for(int i=0;i<vertices.size();i++){
        //当前处理的边是12
        //但是同时取出三条边进行判断
        int x0 = vertices[(i - 1 + vertices.size()) % vertices.size()].x;
        int x1 = vertices[i].x;
        int x2 = vertices[(i + 1) % vertices.size()].x;
        int x3 = vertices[(i + 2) % vertices.size()].x;
        int y0 = vertices[(i - 1 + vertices.size()) % vertices.size()].y;
        int y1 = vertices[i].y;
        int y2 = vertices[(i + 1) % vertices.size()].y;
        int y3 = vertices[(i + 2) % vertices.size()].y;
        //水平线直接舍弃
        if (y1 == y2)
            continue;
        //分别计算下端点y坐标、上端点y坐标、下端点x坐标和斜率倒数
        int ymin = min(y1,y2);
        int ymax = max(y1,y2);
        float x = y1 > y2 ? x2 : x1;
        float dx = (x1 - x2) * 1.0f / (y1 - y2);
        //奇点特殊处理，若点2->1->0的y坐标单调递减则y1为奇点，若点1->2->3的y坐标单调递减则y2为奇点
        //将非极值顶点断开一个像素
        if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3)))
        {
            ymin++;
            x += dx;
        }
        ET[ymin].push_front(edge(ymax,x,dx));
    }

    //开始从下往上进行扫描
    for(int i=miny;i<maxy;i++){
        //取出ET中当前扫描行的所有边并按x的递增顺序（若x相等则按dx的递增顺序）插入AET
        for(auto e:ET[i]){
            bool inserted=false;
            for(auto it=AET.begin();it!=AET.end();it++){
                if(it->xmin>e.xmin||it->xmin==e.xmin&&it->dx>e.dx){
                    AET.insert(it,e);
                    inserted=true;
                    break;
                }
            }
            //不是在链表中插入则在链表尾插入
            if(!inserted)
                AET.push_back(e);
        }
        //AET中的边两两配对并填色

        for(int j=0;j<AET.size()-1;j+=2){
            //一对之间的所有点着色
            for(int xx=AET[j].xmin;xx<=AET[j+1].xmin;xx++){
                painter.drawPoint(xx,i);
            }
        }
        //删除AET中满足y=ymax的边
        auto it=AET.begin();
        while(it!=AET.end()){
            if(it->ymax==i)
                it=AET.erase(it);
            else
                it++;
        }
        //更新AET中边的x值，进入下一循环
        it=AET.begin();
        while(it!=AET.end()){
            it->xmin+=it->dx;
            it++;
        }
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    polygonScan(painter);
}
