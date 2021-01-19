#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w(20, 40, 30);
    w.setWindowTitle("Points");
    w.show();
    return a.exec();
}
