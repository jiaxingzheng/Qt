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
#include <QList>
#include <QDebug>

#include "HuffTree.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    QLabel *title;
    QToolButton *open;
    QToolButton *encode;
    QWidget *widget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *middleLayout;
    QString fileName;
    QStringList list;

    template <typename E> HuffTree<E>* buildHuff(HuffTree<E>** TreeArray,int count);
    template <typename E> void traverse(HuffNode<E>* root,QString str);
    template <typename E> int getMin(HuffTree<E>** TreeArray,int count);
    template <typename E> void remove(HuffTree<E>** TreeArray,int count,int i);
private slots:
    void chooseFile();
    void code();



};

#endif // MAINWINDOW_H
