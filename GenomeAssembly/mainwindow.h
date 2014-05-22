#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QToolButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QTime>
#include <algorithm>
#include <QTableWidget>
#include <QHeaderView>
#include "hashtable.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    QLabel *title;
    QToolButton *open;
    QToolButton *assemble;
    QWidget *widget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *middleLayout;
    QString fileName;
    QTableWidget *table;
    int rowCount;






private slots:
    void chooseFile();
    void code();

    void openCodeFile(QString filename);

};

#endif // MAINWINDOW_H
