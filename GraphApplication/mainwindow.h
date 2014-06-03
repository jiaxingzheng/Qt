#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define UNVISITED 0
#define VISITED 1

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QQueue>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDesktopServices>
#include <QUrl>
#include "Graphm.h"


struct spot{
    QString name;
    QString introduction;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void initData();
    void DFS(Graph* G,int v);
    void BFS(Graph* G, int start, QQueue<int>* Q);
    void visit(int v);
    void initMark();
    void shortestPath_Floyd();
    void getShortestPath();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *map;
    QPushButton *guideButton;
    QPushButton *traverseButton;

    Graphm *graph;
    int **pathmatrix;
    int **shortestPathTable;

    int startIndex;
    int endIndex;

    spot* spots;
    QLabel* shortestPath;
    QLabel* startInfo;
    QLabel* endInfo;

    QString string;
    QStringList list;


public slots:
    void traverse();
    void guide();

    void changeStartIndex(int index);
    void changeEndIndex(int index);
    void openFile(QString filename);


};

#endif // MAINWINDOW_H
