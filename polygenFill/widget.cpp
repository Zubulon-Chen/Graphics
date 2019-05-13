#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ET.resize(this->height());
    ss=INPUT;
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
        int x1 = vertices[i].x;
        int x2 = vertices[(i + 1) % vertices.size()].x;
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
        //此时在该点的水平线的填充会由下面条边完成
        //倘若不进行此步处理，则该点处的上下两条边都加入，当前时刻会AET中会有三个项
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
            auto it=lower_bound(AET.begin(),AET.end(),e);
            AET.insert(it,e);
        }
        //AET中的边两两配对并填色
        for(int j=0;j<AET.size()-1;j+=2){
            //一对之间的所有点着色
            painter.drawLine(AET[j].xmin,i,AET[j+1].xmin,i);
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
    if(ss==INPUT){
        if(vertices.empty())
            return;
        painter.setPen(QPen(Qt::red,10));
        for(auto p:vertices){
            painter.drawPoint(p.x,p.y);
        }
    }
    else if(ss==OVER){
        polygonScan(painter);
    }
}

void Widget::on_input_Button_clicked()
{
    vertices.clear();
    for(auto& v:ET){
        v.clear();
    }
    AET.clear();
    ss=INPUT;
    update();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(ss==INPUT){
            vertices.push_back(point(event->pos().x(),event->pos().y()));
            update();
        }
    }
}

void Widget::on_over_Button_clicked()
{
    ss=OVER;
    update();
}
