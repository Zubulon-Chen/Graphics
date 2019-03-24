#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVector<point> vertices = { point(395, 887), point(479, 998), point(1199, 433),
                               point(1101, 867), point(1294, 715), point(1417, 171),
                               point(857, 163), point(668, 314), point(1111, 321)};

    Widget w(nullptr,vertices,1800,1200);
    w.show();

    return a.exec();
}
