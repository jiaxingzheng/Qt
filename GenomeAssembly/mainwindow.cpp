#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    rowCount=0;
    mainLayout = new QVBoxLayout;
    widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    //setStyleSheet("background-color:rgb(192,192,192)");

    title = new QLabel("Genome Assembly");
    title->setStyleSheet("font-size:25px");
    mainLayout->addWidget(title);
    title->setAlignment(Qt::AlignHCenter);

    open = new QToolButton;
    open->setText("Click to choose a file...");
    open->setStyleSheet("color:white;background-color:rgba(1,1,1,1);border-style:inset");
    connect(open,SIGNAL(clicked()),this, SLOT(chooseFile()));
    open->setFixedSize(380,30);


    assemble = new QToolButton;
    assemble ->setText("Assemble");
    assemble->setStyleSheet("color:white;background-color:rgba(1,1,1,1);border-style:inset");
    assemble->setFixedHeight(30);
    connect(assemble,SIGNAL(clicked()),this,SLOT(code()));


    middleLayout = new QHBoxLayout;
    middleLayout->addWidget(open);
    middleLayout->addWidget(assemble);
    mainLayout->addLayout(middleLayout);

    table = new QTableWidget(0,8);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置为选择整行
    QStringList header;
    header.append(tr("ID"));
    header.append(tr("Contigs Number"));
    header.append(tr("Total Length"));
    header.append(tr("Maximal Length"));
    header.append(tr("N50 Length"));
    header.append(tr("Mean Length"));
    header.append(tr("Median Length"));
    header.append(tr("Time(ms)"));
    table->setHorizontalHeaderLabels(header);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑表格
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并且平分
    table->setTextElideMode(Qt::ElideMiddle);
    table->verticalHeader()->setVisible(false);

    mainLayout->addWidget(table);



    mainLayout->addStretch(0);


}

MainWindow::~MainWindow()
{

}
void MainWindow::chooseFile()
{
    QFileDialog *fd = new QFileDialog;


    fileName = fd->getOpenFileName();

    if(!fileName.isEmpty())
        open->setText(fileName);
}
void MainWindow::code()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            QMessageBox::information(this,"Error","Can't open the file!");
            return;
    }

    QTextStream in(&file);
    QStringList text;

    int i=0;
    while(!in.atEnd())
    {
        if(i==1)
            text.append(in.readLine());
        else
            in.readLine();
        i++;
        if(i==4)
            i=0;
    }

    //qDebug()<<text;
    file.close();

    HashTable *hashTable = new HashTable;
    unsigned int FirPrefix[1048576];
    unsigned int count=0;

    for(i=0;i<text.length();i++)
    {
        for(int j=0;j<65;j++)
        {
            hashTable->addTable(text[i].mid(j,24));
        }
    }


    /*for(i=0;i<1048576;i++)
    {
        hashTable->locate(i);
        if(hashTable->getFreInRead()!=0)
        {
            FirPrefix[count]=i;
            count++;
        }
    }

    unsigned int countFreInRead[9];
    for(i=0;i<9;i++)
        countFreInRead[i]=0;
    for(unsigned int i=0;i<count;i++)
    {
        hashTable->locate(FirPrefix[i]);
        unsigned short freInRead=hashTable->getFreInRead();
        //qDebug()<<freInRead<<endl;
        for(int j=10;j<=90;j+=10)
        {
            if(freInRead<=j&&(freInRead>j-10))
            {
                countFreInRead[j/10-1]++;
                //qDebug()<<"countFreInRead["<<j/10-1<<"]="<<countFreInRead[j/10-1];
            }

        }

    }

    for(i=0;i<9;i++)
    {
        qDebug()<<i*10<<"~"<<(i+1)*10<<"  "<<countFreInRead[i]<<endl;
    } //研究发现，fre 值在 1~30 之间分布较集中*/


    /*qDebug()<<"--------------------------"<<endl;
    hashTable->locate(955561);
    hashTable->getFirPrefix();*/

    for(i=0;i<1048576;i++)
    {
        hashTable->locate(i);
        if(hashTable->getFreInRead()<=30&&hashTable->getFreInRead()>20)
        {
            FirPrefix[count]=i;
            //qDebug()<<FirPrefix[count]<<endl;
            count++;

        }
    }




    qDebug()<<"--------------------------------";


    QTime t;
    t.start();


    QStringList contigList;
    for(unsigned int i=0;i<count;i++)
    {

        //qDebug()<<"for"<<endl;
        hashTable->locate(FirPrefix[i]);
        //qDebug()<<FirPrefix[i]<<endl;
        if(hashTable->getFreInContig()==1)
            continue;

        QString contig;
        contig+=hashTable->getFirPrefix();
        contig+=hashTable->getSecPrefix();
        while(hashTable->getFreInRead()!=0&&hashTable->getFreInContig()!=1)
        {

            //qDebug()<<"while"<<endl;
            hashTable->setFreInContig();


            contig+=hashTable->getPostfix();


            //qDebug()<<contig<<endl;


            unsigned int next = hashTable->hash(contig.mid(contig.length()-20,10));

            //qDebug()<<next<<endl;
            hashTable->locate(next);

        }
        contigList.append(contig);

    }
    int time = t.elapsed();




    QString outputFileName(fileName);
    outputFileName+=".assembly";
    QFile outputFile(outputFileName);


    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this,"Error","Can't create the assembly file!");
        return;
    }
    QTextStream out(&outputFile);



    unsigned int totalLength=0;
    unsigned int maximalLength=0;
    unsigned int length[contigList.length()];
    unsigned int medianLength;

    unsigned int N50Length;
    for(unsigned int i=0;i<contigList.length();i++)
    {
        out<<contigList[i]<<endl;
        totalLength+=contigList[i].length();
        length[i]=contigList[i].length();
        if(contigList[i].length()>maximalLength)
            maximalLength=contigList[i].length();

    }
    unsigned int meanLength = totalLength/contigList.length();
    std::sort(length,length+contigList.length());
    for(unsigned int i=0;i<contigList.length();i++)
        //qDebug()<<length[i]<<endl;

    if(contigList.length()%2==0)
        medianLength = (length[contigList.length()/2]+length[contigList.length()/2-1])/2;
    else
        medianLength = length[contigList.length()/2];

    unsigned int sum=0;
    //qDebug()<<length[contigList.length()-1];
    for(unsigned int i=contigList.length()-1;i>=0;i--)
    {
        //qDebug()<<"for";
        sum=length[i]+sum;
        //qDebug()<<i;
        if(sum>=totalLength/2)
        {
            N50Length = length[i];
            break;
        }
    }



    rowCount++;

    table->setRowCount(rowCount);
    table->setItem(table->rowCount()-1,0,new QTableWidgetItem(QString::number(rowCount)));
    table->setItem(table->rowCount()-1,1,new QTableWidgetItem(QString::number(contigList.length())));
    table->setItem(table->rowCount()-1,2,new QTableWidgetItem(QString::number(totalLength)));
    table->setItem(table->rowCount()-1,3,new QTableWidgetItem(QString::number(maximalLength)));
    table->setItem(table->rowCount()-1,4,new QTableWidgetItem(QString::number(N50Length)));
    table->setItem(table->rowCount()-1,5,new QTableWidgetItem(QString::number(meanLength)));
    table->setItem(table->rowCount()-1,6,new QTableWidgetItem(QString::number(medianLength)));
    table->setItem(table->rowCount()-1,7,new QTableWidgetItem(QString::number(time)));



     QString dir("ID ");
     dir+=QString::number(rowCount);
     dir+="  finished assembling  ";
     dir+="<a href = ";
     dir += outputFileName;
     dir += ">";
     dir += outputFileName;
     dir +="</a>";
     QLabel *fileDir = new QLabel(dir);
     connect(fileDir,SIGNAL(linkActivated(QString)),this,SLOT(openCodeFile(QString)));

     mainLayout->addWidget(fileDir);

     QMessageBox::information(this,"Success","Assemble success!");





}
void MainWindow::openCodeFile(QString filename)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

}
