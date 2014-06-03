#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setFixedSize(800,500);
    scene = new QGraphicsScene;
    view = new QGraphicsView(scene);
    setCentralWidget(view);
    map = new QGraphicsPixmapItem(QPixmap(":/images/background.jpg"));


    scene->addItem(map);
    traverseButton = new QPushButton(tr("Traverse"));
    connect(traverseButton,SIGNAL(clicked()),this,SLOT(traverse()));
    scene->addWidget(traverseButton);
    guideButton = new QPushButton(tr("Guide"));
    connect(guideButton,SIGNAL(clicked()),this,SLOT(guide()));
    scene->addWidget(guideButton);
    guideButton->setGeometry(100,0,100,30);
    scene->update();
    view->update();


    initData();


}

MainWindow::~MainWindow()
{

}
void MainWindow::initData()
{
    QFile file("config");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "warning","Cannot write file");
        qDebug()<<file.error();
        qDebug()<<file.errorString();
    }
    QString text = file.readAll();
    file.close();
    qDebug()<<text;
    QStringList textList = text.split('\n');
    textList.removeLast();
    graph = new Graphm(textList.length());
    qDebug()<<textList.length();
    for(int i=0;i<textList.length();i++)
    {
        qDebug()<<textList[i]<<endl;
        QStringList templist = textList[i].split(' ');
        qDebug()<<templist.length()<<endl;
        graph->setEdge(templist[0].toInt(),templist[1].toInt(),templist[2].toInt());
    }

    spots = new spot[20];
    file.setFileName("info");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "warning","Cannot write file");
        qDebug()<<file.error();
        qDebug()<<file.errorString();
    }
    text.clear();
    text = file.readAll();
    file.close();
    qDebug()<<text;
    textList.clear();
    textList = text.split('\n');
    textList.removeLast();
    for(int i=0;i<textList.length();i++)
    {
        qDebug()<<textList[i]<<endl;
        QStringList templist = textList[i].split(',');
        qDebug()<<templist.length()<<endl;
        spots[templist[0].toInt()].name = templist[1];
        list.append(templist[1]);
        spots[templist[0].toInt()].introduction = templist[2];
    }

     int n=graph->n();
    shortestPathTable= (int**) new int*[n];
    pathmatrix = (int**) new int*[n]; // Make matrix
    for (int i=0; i<n; i++)
    {
       pathmatrix[i] = new int[n];
       shortestPathTable[i] = new int[n];
    }
    shortestPath_Floyd();


    startIndex=0;
    endIndex=0;
}
void MainWindow::DFS(Graph* G, int v) { // Depth first search

    visit(v);
    G->setMark(v, VISITED);
    for (int w=G->first(v); w<G->n(); w = G->next(v,w))
        if (G->getMark(w) == UNVISITED)
            DFS(G, w);


}
void MainWindow::BFS(Graph* G, int start, QQueue<int>* Q) {
    int v, w;
    Q->enqueue(start);
    // Initialize Q
    G->setMark(start, VISITED);
    while (Q->length() != 0) { // Process all vertices on Q
        v = Q->dequeue();
        visit(v);
        // Take appropriate action
        for (w=G->first(v); w<G->n(); w = G->next(v,w))
            if (G->getMark(w) == UNVISITED) {
                G->setMark(w, VISITED);
                Q->enqueue(w);
            }
    }
}
void MainWindow::traverse()
{
    string.clear();
    initMark();

    QQueue<int>* queue = new QQueue<int>;
    qDebug()<<"BFS"<<endl;
    BFS(graph,0,queue);

    string.chop(3);
    QLabel *BFSResult = new QLabel(string);
    string.clear();

    initMark();
    qDebug()<<"DFS"<<endl;
    DFS(graph,0);
    string.chop(3);
    QLabel *DFSResult = new QLabel(string);

    QDialog *traverseDialog = new QDialog();
    traverseDialog->setFixedSize(800,400);
    traverseDialog->setWindowTitle("Traverse");
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *title = new QLabel("Traverse");
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("font-size:50px");
    QLabel *BFSLabel = new QLabel("BFS:");
    BFSLabel->setStyleSheet("font-size:25px");
    QLabel *DFSLabel = new QLabel("DFS:");
    DFSLabel->setStyleSheet("font-size:25px");
    layout->addWidget(title);
    layout->addWidget(BFSLabel);
    layout->addWidget(BFSResult);
    layout->addWidget(DFSLabel);
    layout->addWidget(DFSResult);
    traverseDialog->setLayout(layout);
    traverseDialog->open();

}
void MainWindow::visit(int v)
{

    qDebug()<<v<<endl;
    string+=spots[v].name;
    string+="-->";

}
void MainWindow::initMark()
{
    for(int i=0;i<graph->n();i++)
        graph->setMark(i,UNVISITED);
}
void MainWindow::guide()
{
    QDialog *guideDialog = new QDialog();
    guideDialog->setFixedSize(500,400);
    guideDialog->setWindowTitle("Guide");
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *title = new QLabel("Guide");
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("font-size:50px");

    QLabel *startLabel = new QLabel("Start:");
    QComboBox *start = new QComboBox;
    start->addItems(list);
    connect(start,SIGNAL(currentIndexChanged(int)),this,SLOT(changeStartIndex(int)));


    QLabel *endLabel = new QLabel("End:");
    QComboBox *end = new QComboBox;
    end->addItems(list);
    connect(end,SIGNAL(currentIndexChanged(int)),this,SLOT(changeEndIndex(int)));

    QHBoxLayout *startLayout = new QHBoxLayout;
    QHBoxLayout *endLayout = new QHBoxLayout;
    startLayout->addWidget(startLabel);
    startLayout->addWidget(start);
    endLayout->addWidget(endLabel);
    endLayout->addWidget(end);
    
    startInfo = new QLabel;
    startInfo->setText("Start Info : "+spots[startIndex].introduction);
    connect(startInfo,SIGNAL(linkActivated(QString)),this,SLOT(openFile(QString)));

    endInfo = new QLabel;
    endInfo->setText("End Info :"+spots[endIndex].introduction);
    connect(endInfo,SIGNAL(linkActivated(QString)),this,SLOT(openFile(QString)));

    shortestPath = new QLabel;

    layout->addWidget(title);
    layout->addLayout(startLayout);
    layout->addWidget(startInfo);
    layout->addLayout(endLayout);
    layout->addWidget(endInfo);
    layout->addWidget(shortestPath);
    layout->addStretch();

    guideDialog->setLayout(layout);
    guideDialog->open();
}
void MainWindow::changeStartIndex(int index)
{
    startIndex=index;
    startInfo->setText("Start Info : "+spots[startIndex].introduction);
    getShortestPath();
}
void MainWindow::changeEndIndex(int index)
{
    endIndex=index;
    endInfo->setText("End Info : "+spots[endIndex].introduction);
    getShortestPath();
}

void MainWindow::getShortestPath()
{          
        qDebug()<<"getShortestPath";
        QString  sp;
        sp+="The shortest path length is ";
        qDebug()<<"The shortest path length is ";
        sp+=QString::number(shortestPathTable[startIndex][endIndex]);
        qDebug()<<shortestPathTable[startIndex][endIndex];
        sp+="\n";
        sp+="The shortest path is";
        sp+="\n";

        sp+=spots[startIndex].name;
        int k=pathmatrix[startIndex][endIndex];
        int w=endIndex;
        qDebug()<<"while";
        while(k!=w)
        {
            sp+="->";
            sp+=spots[k].name;
            k=pathmatrix[k][w];
        }
        sp+="->";
        sp+=spots[w].name;
        shortestPath->setText(sp);
}
void MainWindow::shortestPath_Floyd()
{
    int v,w,k;
    for(v=0;v<graph->n();++v)
    {
        for(w=0;w<graph->n();++w)
        {
            shortestPathTable[v][w]=graph->weight(v,w);
            qDebug()<<shortestPathTable[v][w];
            pathmatrix[v][w]=w;
            qDebug()<<pathmatrix[v][w];
        }
    }
    for(k=0;k<graph->n();++k)
    {
        for(v=0;v<graph->n();++v)
        {
            for(w=0;w<graph->n();++w)
            {
                if((shortestPathTable[v][w]>shortestPathTable[v][k]+shortestPathTable[k][w]||shortestPathTable[v][w]==0)
                        &&shortestPathTable[v][k]!=0&&shortestPathTable[k][w]!=0)
                {
                    shortestPathTable[v][w]=shortestPathTable[v][k]+shortestPathTable[k][w];
                    pathmatrix[v][w]=pathmatrix[v][k];
                }
            }
        }
    }
}
void MainWindow::openFile(QString filename)
{
    qDebug()<<filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}
