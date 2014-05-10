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
        qDebug()<<"#";
        QStringList templist = text.split("#");
        QString head = templist.at(0);
        text = templist.at(1);
        qDebug()<<"head:"<<head;
        qDebug()<<text;
        QLabel *FHL = new QLabel("File Head Length: "+QString::number(head.length()));
        QLabel *ADL = new QLabel("Actual Data Length: "+QString::number(text.length()));

        IntlNode<QChar>* root = new IntlNode<QChar>();
        HuffNode<QChar>* temp = root;


        qDebug()<<"length"<<head.length();
        for(int i=1;i<head.length();i++)
        {
            qDebug()<<"i"<<i;

                if(head.at(i-1)=='0')
                {
                     qDebug()<<"left";


                    if(head.at(i)!='0'&&head.at(i)!='1')
                    {
                        qDebug()<<i<<"letter";
                        LeafNode<QChar>* leafNode = new  LeafNode<QChar>(head.at(i));
                        if(head.at(i-1)=='0')
                        {
                            dynamic_cast<IntlNode<QChar>* >(temp)->setLeft(leafNode);
                            temp = root;
                        }
                        else
                        {
                            dynamic_cast<IntlNode<QChar>* >(temp)->setRight(leafNode);
                            temp = root;
                        }
                        continue;
                    }
                    if(dynamic_cast<IntlNode<QChar>* >(temp)->left()==NULL)
                    {
                        IntlNode<QChar> *intlNode = new IntlNode<QChar>();
                        dynamic_cast<IntlNode<QChar>* >(temp)->setLeft(intlNode);
                        temp = intlNode;
                    }
                    else
                    {
                        temp = dynamic_cast<IntlNode<QChar>* >(temp)->left();
                    }
                }
                else if(head.at(i-1)=='1')
                {
                    qDebug()<<"right";



                    if(head.at(i)!='0'&&head.at(i)!='1')
                    {
                        qDebug()<<i<<"letter";
                        LeafNode<QChar>* leafNode = new  LeafNode<QChar>(head.at(i));
                        if(head.at(i-1)=='0')
                        {
                            dynamic_cast<IntlNode<QChar>* >(temp)->setLeft(leafNode);
                            temp = root;
                        }
                        else
                        {
                            dynamic_cast<IntlNode<QChar>* >(temp)->setRight(leafNode);
                            temp = root;
                        }
                        continue;

                    }

                    if(dynamic_cast<IntlNode<QChar>* >(temp)->right()==NULL)
                    {

                        IntlNode<QChar> *intlNode = new IntlNode<QChar>();
                        dynamic_cast<IntlNode<QChar>* >(temp)->setRight(intlNode);
                        temp = intlNode;
                    }
                    else
                    {

                        temp = dynamic_cast<IntlNode<QChar>* >(temp)->right();

                    }

                }
            }


        qDebug()<<"finish buildingTree";

        QString decode;
        for(int j=0;j<text.length();j++)
        {
            qDebug()<<"j"<<j;
            qDebug()<<"text"<<text.at(j);
            if(text.at(j) == '0')
            {
                qDebug()<<"left";
                temp = dynamic_cast<IntlNode<QChar>* >(temp)->left();
                if(temp->isLeaf())
                {
                    decode+= dynamic_cast<LeafNode<QChar>* >(temp)->val();

                    temp = root;

                }

            }
            else
            {
                qDebug()<<"right";
                temp = dynamic_cast<IntlNode<QChar>* >(temp)->right();
                if(temp->isLeaf())
                {
                    decode+= dynamic_cast<LeafNode<QChar>* >(temp)->val();
                    temp = root;

                }
            }
             qDebug()<<decode;
        }

        QString outputFileName(fileName);
        outputFileName+=".decode";
        QFile outputFile(outputFileName);


        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this,"Error","Can't create the encode file!");
            return;
        }

        qDebug()<<"open";
         QTextStream out(&outputFile);
         out<<decode;


         QHBoxLayout *layout = new QHBoxLayout;
         layout->addWidget(FHL);
         layout->addWidget(ADL);
         mainLayout->addLayout(layout);

         QString dir("Decode    ");
         dir+="<a href = ";
         dir += outputFileName;
         dir += ">";
         dir += outputFileName;
         dir +="</a>";
         QLabel *fileDir = new QLabel(dir);
         connect(fileDir,SIGNAL(linkActivated(QString)),this,SLOT(openCodeFile(QString)));

         mainLayout->addWidget(fileDir);

         QMessageBox::information(this,"Success","Decode success!");



    }
    else
    {
        qDebug()<<"else";
        QLabel *fl = new QLabel("File Length: "+QString::number(text.length()));
        HuffTree<QChar>** TreeArray = new HuffTree<QChar>*[52];
        int count=0;


        for(int i=0;i<26;i++)
        {
            if(text.contains(QChar(i+97))||text.contains(QChar(i+65)))
            {
                QChar ch(i+97);
                qDebug()<<ch;
                int countLetter = text.count(QString(ch));
                qDebug()<<countLetter;
                TreeArray[count] = new HuffTree<QChar>(ch,countLetter);
                qDebug()<<TreeArray[count]->weight();
                count++;
            }
            if(text.contains(QChar(i+65)))
            {
                QChar ch(i+65);
                qDebug()<<ch;
                int countLetter = text.count(QString(ch));
                qDebug()<<countLetter;
                TreeArray[count] = new HuffTree<QChar>(ch,countLetter);
                qDebug()<<TreeArray[count]->weight();
                count++;
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
        qDebug()<<str;

        qDebug()<<fileName;
        QString outputFileName(fileName);
        outputFileName+=".encode";
        QFile outputFile(outputFileName);


        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this,"Error","Can't create the encode file!");
            return;
        }

        qDebug()<<"open";
         QTextStream out(&outputFile);
         out<<str;

         mainLayout->addWidget(fl);
         QString dir("Encode    ");
         dir+="<a href = ";
         dir += outputFileName;
         dir += ">";
         dir += outputFileName;
         dir +="</a>";
         QLabel *fileDir = new QLabel(dir);
         connect(fileDir,SIGNAL(linkActivated(QString)),this,SLOT(openCodeFile(QString)));

         mainLayout->addWidget(fileDir);
         QMessageBox::information(this,"Success","Encode success!");


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
        qDebug()<<"return";
        return;

    }
    str+="0";
    traverse(dynamic_cast<IntlNode<E>* >(root)->left(),str);
    str.chop(1);

    qDebug()<<"str"<<str;
    str+="1";
    traverse(dynamic_cast<IntlNode<E>* >(root)->right(),str);


    str.chop(1);
     qDebug()<<"str"<<str;
    return;
}
void MainWindow::openCodeFile(QString filename)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

}
