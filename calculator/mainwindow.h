#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QTableWidget>
#include <QPalette>
#include <QColor>
#include <QStack>
#include <QDebug>
#include <QHeaderView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *widget;
    QVBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QGridLayout *rightLayout;
    QHBoxLayout *bottomLayout;
    QLineEdit *lineEdit;
    QTableWidget *tableWidget;
    QToolButton *digitButtons[10];
    QToolButton *symbolButtons[9];
    QToolButton *changeView;

    bool flag;
    bool buttonFlag;

    QStringList stackChange;
    QStringList calculateProcess;




    void initUI();
    QToolButton *createButtons(const QString &text);
    void calculate();

private slots:
    void buttonClicked();
    void viewChange();


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
