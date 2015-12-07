#include "mainwindow.h"
#include <QApplication>
#include <QGst/Init>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QGst::init(&argc, &argv);
    w.show();

    return a.exec();
}
