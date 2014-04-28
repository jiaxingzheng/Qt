#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("商品订购管理软件");
    w.setMinimumSize(QSize(800,500));
    w.move(QApplication::desktop()->rect().center()-QPoint(400,250));
    w.show();

    return a.exec();
}

