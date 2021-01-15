#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Points");
    w.show();
    return a.exec();
}
