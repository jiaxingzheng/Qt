#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedHeight(500);
    w.setFixedWidth(450);
    w.move(QApplication::desktop()->rect().center()-QPoint(200,250));
    w.show();

    return a.exec();
}
