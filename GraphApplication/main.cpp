#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(resource);

    QApplication app(argv, args);
    MainWindow mainWindow;
    mainWindow.move(QApplication::desktop()->rect().center()-QPoint(400,250));
    mainWindow.show();


    return app.exec();
}
