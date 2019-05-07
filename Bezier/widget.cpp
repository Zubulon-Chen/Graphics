#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //    setFixedSize(1000,800);
    //    points={QPoint(100,300),QPoint(250,100),QPoint(400,100),QPoint(600,500)};
    idx=0;
    change=false;
    click_points=true;
    delete_point=false;
    isBezier=true;
    ui->radioButton->setChecked(true);
    degree=3;
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
    QPoint prev;
    bool first=true;
    QVector<QPoint> pp;
    painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for(double t=0.0;t<=1.0;t+=0.01){
        points=ppp;
        while(points.size()>1){
            for(int i=0;i<points.size()-1;i++){
                pp.push_back(points[i]*(1-t)+points[i+1]*t);
            }
            points=pp;
            pp.clear();
        }
        if(first){
            prev=points.front();
            first=false;
        }
        else{
            painter.drawLine(prev,points.front());
            prev=points.front();
        }
        //        painter.drawPoint(points.front());
    }
    points=ppp;
}

void Widget::drawB_Spline(QPainter &painter)
{
    QPoint prev;
    bool first=true;
    int p=degree-1;
    int n=points.size();
    QVector<int> knots(n+p+1);
    //均匀分布的节点
    //其中第一段和最后一段会缺失
    //    for(int i=0;i<knots.size();i++)
    //        knots[i]=i;
    //clamped效果
    //第一个和最后一个节点重复度p+1
    //中间有n-p-1个节点均匀分布
    for(int i=0;i<p+1;i++)
        knots[i]=0;
    for(int i=p+1;i<n;i++)
        knots[i]=i-p;
    for(int i=n;i<n+p+1;i++)
        knots[i]=n-p;
    QVector<QPoint> pp;
    painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //u定义域在[up,um-p-1]，即[up,un]
    for(double u=knots[p];u<knots[points.size()];u+=0.001){
        pp.clear();
        //找到当前u在哪个节点区间内
        int k=std::distance(knots.begin(),std::upper_bound(knots.begin(),knots.end(),u))-1;
        //区间[uk,uk+1)对应的控制顶点[Pk-p,Pk]
        for(int i=k-p;i<k+1;i++)
            pp.push_back(points[i]);
        //外层for是三角形从左向右
        //内层for是每一列自上而下
        //pp数组每次自后向前更新
        for(int r=1;r<=p;r++){
            for (int i = k,j=p;i >= k - p+r;--i,--j) {
                double alpha = u - knots[i];
                double dev = (knots[i+p+1-r]-knots[i]);
                alpha = (dev !=0)? alpha / dev : 0;
                pp[j] = (1.0 - alpha)*pp[j - 1] + alpha * pp[j];
            }
        }
        if(first){
            prev=pp.back();
            first=false;
        }
        else{
            painter.drawLine(prev,pp.back());
            prev=pp.back();
        }
        //        painter.drawPoint(pp.back());
    }
}



void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::darkGreen);
    pen.setWidth(10);
    painter.setPen(pen);
    for(auto point:points){
        painter.drawPoint(point);
    }
    if(!click_points){
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        drawLines(points,painter);
        if(isBezier)
            drawBezier(painter);
        else
            drawB_Spline(painter);
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(click_points){
            points.push_back(event->pos());
            update();
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
    QString text=QString("%1 %2").arg(event->pos().x()).arg(event->pos().y());
    QToolTip::showText(QPoint(event->globalPos()),text,this);  //显示鼠标坐标,更随鼠标用方框的形式
    if(change){  //改变点的位置，paintEvent调用update实时更新
        points[idx]=QPoint(event->pos().x(),event->pos().y());
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    change=false;
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

void Widget::on_radioButton_toggled(bool checked)
{
    if(checked){
        isBezier=true;
        update();
    }
}

void Widget::on_radioButton_2_toggled(bool checked)
{
    if(checked){
        isBezier=false;
        bool ok;
        int i = QInputDialog::getInt(this, tr("Input degree"),
                                     tr("input degree: "), 4, 0, 100, 1, &ok);
        if (ok){
            //输入的阶数应当小于等于顶点数量，否则u定义域为空
            if(i<=numPoints)
                degree=i;
            else{
                QMessageBox::warning(NULL, "warning", "Please input right k",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
        }
        update();
    }
}

void Widget::on_pushButton_3_clicked()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("change degree"),
                                 tr("input degree: "), 4, 0, 100, 1, &ok);
    if (ok){
        //输入的阶数应当小于等于顶点数量，否则u定义域为空
        if(i<=numPoints)
            degree=i;
        else{
            QMessageBox::warning(NULL, "warning", "Please input right k",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    }
    update();
}
