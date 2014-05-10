#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedHeight(300);
    w.setFixedWidth(600);
    w.move(QApplication::desktop()->rect().center()-QPoint(300,150));
    w.show();


    return a.exec();
}
