#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainLayout = new QVBoxLayout;
    widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    //setStyleSheet("background-color:rgb(192,192,192)");

    title = new QLabel("Encode/Decode System");
    title->setStyleSheet("font-size:25px");
    mainLayout->addWidget(title);
    title->setAlignment(Qt::AlignHCenter);

    open = new QToolButton;
    open->setText("Click to choose a file...");
    open->setStyleSheet("color:white;background-color:rgba(1,1,1,1);border-style:inset");
    connect(open,SIGNAL(clicked()),this, SLOT(chooseFile()));
    open->setFixedSize(380,30);


    encode = new QToolButton;
    encode ->setText("Encode/Decode");
    encode->setStyleSheet("color:white;background-color:rgba(1,1,1,1);border-style:inset");
    encode->setFixedHeight(30);
    connect(encode,SIGNAL(clicked()),this,SLOT(code()));


    middleLayout = new QHBoxLayout;
    middleLayout->addWidget(open);
    middleLayout->addWidget(encode);
    mainLayout->addLayout(middleLayout);


    mainLayout->addSpacing(200);

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
    qDebug()<<"code";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            QMessageBox::information(this,"Error","Can't open the file!");
            return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    qDebug()<<text;
    file.close();
    if(text.contains('#'))
    {
        QStringList temp = text.split("#");
        QString head = temp.at(0);
        text = temp.at(1);
        QLabel fhl("File Head Length: "+QString::number(head.length()));
        QLabel ADL("Actual Data Length: "+QString::number(text.length()));



    }
    else
    {
        qDebug()<<"else";
        QLabel fl("File Length: "+QString::number(text.length()));
        HuffTree<QChar>** TreeArray ;
        int count=0;


        for(int i=0;i<26;i++)
        {
            if(text.contains(QChar(i+97)))
            {
                QChar ch(i+97);
                qDebug()<<ch;
                int countLetter = text.count(QString(ch));
                qDebug()<<countLetter;
                TreeArray[count] = new HuffTree<QChar>(ch,countLetter);
                qDebug()<<TreeArray[count]->weight();

                qDebug()<<cout;


            }


        }
        qDebug()<<"TreeArray";
        HuffTree<QChar>* tree = buildHuff(TreeArray,count);
        qDebug()<<"buildHuff";
        HuffNode<QChar>* root = tree->root();
        qDebug()<<"root";
        QString str;
        traverse(root,str);
        str.clear();
        str+="head:";


        for(int i=0;i<list.length();i++)
        {
            str+=list.at(i);
            QString temp = list.at(i);
            QString letter = temp.at(temp.length()-1);
            temp.chop(1);
            text.replace(letter,temp);
        }

        str+="#";
        str+=text;


        QFile outputFile("Encode"+fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
               return;
         QTextStream out(&outputFile);
         out<<text;
    }

}
template <typename E> HuffTree<E>* MainWindow::buildHuff(HuffTree<E>** TreeArray, int count) {
    qDebug()<<"build";
    heap<HuffTree<E>*>* forest = new heap<HuffTree<E>*>(TreeArray,count, count);
    HuffTree<QChar> *temp1, *temp2, *temp3 = NULL;
    qDebug()<<"while";
    while (forest->size() > 1) {
        temp1 = forest->removefirst();
        qDebug()<<temp1->weight();
        temp2 = forest->removefirst();

        temp3 = new HuffTree<E>(temp1, temp2);
        forest->insert(temp3); // Put the new tree back on list
        delete temp1;
// Must delete the remnants
        delete temp2;
    }
    return temp3;
}
template <typename E> void MainWindow::traverse(HuffNode<E>* root,QString str)
{
    if(root->isLeaf())
    {
        str+=dynamic_cast<LeafNode<E>* >(root)->val();
        list.append(str);
        str.chop(2);
        return;
    }
    str+="0";
    traverse(dynamic_cast<IntlNode<E>* >(root)->left(),str);
    str+="1";
    traverse(dynamic_cast<IntlNode<E>* >(root)->right(),str);
    str.chop(1);
    return;

}
