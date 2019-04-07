#include <QApplication>
#include <QLabel>
#include "mainwindow.h"

int main(int argc, char* argv[] ) {
    QApplication a( argc, argv );

    MyWidget m;
    m.show();

    a.connect(&a ,SIGNAL(lastWindowClosed() ), &a, SLOT(quit() ) );
    return a.exec();
}
