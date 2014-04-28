#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    flag = false;
    buttonFlag=false;
    initUI();
}

MainWindow::~MainWindow()
{

}
void MainWindow::initUI()
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window,QColor(0,0,0));
    this->setPalette(pal);

    mainLayout = new QVBoxLayout;

    widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    lineEdit = new QLineEdit;
    lineEdit->setFixedHeight(50);
    //lineEdit->setFixedWidth(this->width());
    pal = lineEdit->palette();
    pal.setColor(QPalette::Base,QColor(1,1,1));
    pal.setColor(QPalette::Text,QColor(225,225,225));
    lineEdit->setPalette(pal);
    lineEdit->setStyleSheet("font-size:40px;border-width:0;");
    lineEdit->setAlignment(Qt::AlignRight);
    QRegExp regExp("[0-9+*/()-.]*");
    QRegExpValidator *validator =new QRegExpValidator(regExp,this);
    lineEdit->setValidator(validator);


    mainLayout->addWidget(lineEdit);

    bottomLayout = new QHBoxLayout;
    mainLayout->addLayout(bottomLayout);
    leftLayout = new QVBoxLayout;

    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(2);
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑表格
    //tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并且平分
    tableWidget->setColumnWidth(0,20);
    tableWidget->setColumnWidth(1,140);
    tableWidget->setShowGrid(false);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFont(QFont("Helvetica [Cronyx]",22,-1,false));
    //tableWidget->setStyleSheet("font-size:20px");
    leftLayout->addWidget(tableWidget);



    changeView = new QToolButton;
    changeView->setText(trUtf8("计算过程"));
    changeView->setMinimumWidth(180);
    changeView->setStyleSheet("background-color:white");
    connect(changeView,SIGNAL(clicked()),this,SLOT(viewChange()));


    leftLayout->addWidget(changeView);

    bottomLayout->addLayout(leftLayout);


    rightLayout = new QGridLayout;
    rightLayout->setSpacing(2);


    for(int i=0;i<10;i++)
    {
        digitButtons[i]=createButtons(QString::number(i));
        digitButtons[i]->setStyleSheet("font-size:30px;color:white;background-color:rgb(71,71,71)");


        //QPalette pal = digitButtons[i]->palette();
        //pal.setColor(QPalette::Button,QColor(112,128,105));
        //pal.setColor(QPalette::ButtonText,QColor(225,225,225));
        //digitButtons[i]->setPalette(pal);
    }

    symbolButtons[0]=createButtons("*");
    symbolButtons[1]=createButtons("/");
    symbolButtons[2]=createButtons("+");
    symbolButtons[3]=createButtons("-");
    symbolButtons[4]=createButtons(".");
    symbolButtons[5]=createButtons("=");
    symbolButtons[6]=createButtons("(");
    symbolButtons[7]=createButtons(")");
    symbolButtons[8]=createButtons(trUtf8("删除"));
    for(int i=0;i<8;i++)
    {
        symbolButtons[i]->setStyleSheet("font-weight:bold;font-size:30px;color:white;background-color:rgb(33,33,33)");
    }
    symbolButtons[8]->setStyleSheet("font-size:20px;color:white;background-color:rgb(26,26,26)");


    bottomLayout->addLayout(rightLayout);




}
QToolButton *MainWindow::createButtons(const QString &text)
{
    QToolButton *tb = new QToolButton;
    tb->setText(text);
    connect(tb,SIGNAL(clicked()),this,SLOT(buttonClicked()));
    tb->setFixedSize(60,83);
    if(text == QString("0"))
        rightLayout->addWidget(tb,4,1);
    else if(text == QString("("))
        rightLayout->addWidget(tb,0,0);
    else if(text == QString(")"))
        rightLayout->addWidget(tb,0,1);
    else if(text == QString(trUtf8("删除")))
        rightLayout->addWidget(tb,0,3);
    else if(text == QString("*"))
        rightLayout->addWidget(tb,1,3);
    else if(text == QString("/"))
        rightLayout->addWidget(tb,2,3);
    else if(text == QString("+"))
        rightLayout->addWidget(tb,3,3);
    else if(text == QString("-"))
        rightLayout->addWidget(tb,4,3);
    else if(text == QString("."))
        rightLayout->addWidget(tb,4,0);
    else if(text == QString("="))
        rightLayout->addWidget(tb,4,2);
    else
        rightLayout->addWidget(tb,(text.toInt()-1)/3+1,(text.toInt()-1)%3);

    return tb;

}
void MainWindow::buttonClicked()
{
    if(flag == true)
    {
        lineEdit->clear();
        flag = false;
    }
    QString expression = lineEdit->text();
    QToolButton *tb = qobject_cast<QToolButton *>(sender());
    QString text = tb->text();

    if(text == QString(trUtf8("删除")))
        lineEdit->backspace();
    else if(text == QString("("))
    {
        lineEdit->cursorBackward(true);
        QString leftLetter = lineEdit->selectedText();
        if(leftLetter == QString("*")||leftLetter == QString("/")||leftLetter == QString("+")||leftLetter == QString("-"))
            lineEdit->setText(expression+=text);
        else
        {
            lineEdit->setText(expression);
            return;
        }

    }
    else if(text == QString(")"))
    {
        lineEdit->cursorBackward(true);
        QString leftLetter = lineEdit->selectedText();
        int countLeft=expression.count("(");
        qDebug()<<countLeft;
        int countRight=expression.count(")");
        qDebug()<<countRight;

        if(leftLetter == QString("*")||leftLetter == QString("/")||leftLetter == QString("+")||leftLetter == QString("-")||leftLetter == QString(".")||leftLetter == QString("(")||countLeft<=countRight)
            lineEdit->setText(expression);
        else
            lineEdit->setText(expression+=text);
    }

    else if(text == QString("*")||text == QString("/")||text == QString("+")||text == QString("-")||text == QString("."))
    {
        lineEdit->cursorBackward(true);
        QString leftLetter = lineEdit->selectedText();
        if(leftLetter == QString("*")||leftLetter == QString("/")||leftLetter == QString("+")||leftLetter == QString("-")||leftLetter == QString(".")|| leftLetter.isEmpty() )
        {
            lineEdit->setText(expression);
            return;
        }
        else
            lineEdit->setText(expression+=text);


    }
    else if(text == QString("="))
        calculate();
    else
        lineEdit->setText(expression+=text);

}
void MainWindow::calculate()
{
    QString expression = lineEdit->text();
    int countLeft=expression.count("(");
    int countRight=expression.count(")");
    if(countRight!=countLeft||expression.endsWith('+')||expression.endsWith('-')||expression.endsWith('*')
            ||expression.endsWith('/')||!(expression.contains('+')||expression.contains('-')||expression.contains('*')||expression.contains('/')))
        return;
    stackChange.clear();
    stackChange.append(QString("infix to"));
    stackChange.append(QString("suffic"));
    QStack<QString> stack;
    QString temp;
    QString temp1;
    QStringList list;
    while(!expression.isEmpty())
    {
        qDebug()<<"expression:"<<expression;
        qDebug()<<temp;
        if(expression.startsWith('+')||expression.startsWith('-')||expression.startsWith('*')||expression.startsWith('/')||expression.startsWith('(')||expression.startsWith(')'))
        {
            qDebug()<<"if";
            qDebug()<<temp;
            if(!temp.isEmpty())
            {
                list.append(temp);
                temp.clear();
            }
            if(expression.startsWith('+')||expression.startsWith('-')||expression.startsWith(')'))
            {
                while(!stack.isEmpty())
                {
                    if(stack.top()==QString('('))
                    {
                        if(expression.startsWith(')'))
                        {
                            stackChange.append(QString("pop"));
                            stackChange.append(stack.top());
                            stack.pop();
                        }
                        break;
                    }
                    else
                    {
                        stackChange.append(QString("pop"));
                        stackChange.append(stack.top());
                        list.append(stack.pop());
                    }
                }

            }
            if(expression.startsWith(')'))
                expression.remove(0,1);
            if(!expression.isEmpty())
            {
                stackChange.append(QString("push"));
                stackChange.append(expression.left(1));
                stack.push(expression.left(1));
                expression.remove(0,1);
            }
            qDebug()<<"stack:"<<stack.top();

        }
        else
        {
            temp+=expression.left(1);
            expression.remove(0,1);
        }

    }
    if(!temp.isEmpty())
        list.append(temp);
    qDebug()<<"out of while";

    while(!stack.isEmpty())
    {
        qDebug()<<"top:"<<stack.top();
        stackChange.append(QString("pop"));
        stackChange.append(stack.top());
        list.append(stack.pop());
    }

    for(int i=0;i<list.length();i++)
        qDebug()<<i<<":"<<list.at(i);

    stackChange.append(QString("calculate"));
    stackChange.append(QString("suffix"));

    QString operand1;
    QString operand2;
    QString result;
    int row=0;
    while(!list.isEmpty())
    {
        qDebug()<<"in while";
        if(list.startsWith("+"))
        {
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand2 = stack.pop();
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand1 = stack.pop();
            result = QString::number(operand1.toDouble()+operand2.toDouble());
            stackChange.append(QString("push"));
            stackChange.append(result);
            stack.push(result);
        }
        else if(list.startsWith("-"))
        {
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand2 = stack.pop();
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand1 = stack.pop();

            result = QString::number(operand1.toDouble()-operand2.toDouble());
            stackChange.append(QString("push"));
            stackChange.append(result);
            stack.push(result);
        }
        else if(list.startsWith("*"))
        {
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand2 = stack.pop();
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand1 = stack.pop();

            result = QString::number(operand1.toDouble()*operand2.toDouble());
            stackChange.append(QString("push"));
            stackChange.append(result);
            stack.push(result);
        }
        else if(list.startsWith("/"))
        {
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand2 = stack.pop();
            if(operand2.toDouble()==0)
            {
                lineEdit->setText(trUtf8("除数不能为0"));
                flag = true;
            }
            stackChange.append(QString("pop"));
            stackChange.append(stack.top());
            operand1 = stack.pop();
            result = QString::number(operand1.toDouble()/operand2.toDouble());
            stackChange.append(QString("push"));
            stackChange.append(result);
            stack.push(result);
        }
        else
        {
            qDebug()<<"else";
            stackChange.append(QString("push"));
            stackChange.append(list.first());
            stack.push(list.first());
            qDebug()<<stack.top();
        }
        if(!operand1.isEmpty()&&!operand2.isEmpty())
        {
            tableWidget->setRowCount(row+3);
            qDebug()<<"operand";
            calculateProcess.append(QString(""));
            calculateProcess.append(operand1);

            QTableWidgetItem *item = new QTableWidgetItem(operand1);
            item->setTextAlignment(Qt::AlignRight);
            tableWidget->setItem(row++,1,item);

            calculateProcess.append(list.first());
            item = new QTableWidgetItem(list.first());
            item->setTextAlignment(Qt::AlignVCenter);
            tableWidget->setItem(row,0,item);


            calculateProcess.append(operand2);
            item = new QTableWidgetItem(operand2);
            item->setTextAlignment(Qt::AlignRight);
            tableWidget->setItem(row++,1,item);

            calculateProcess.append(QString("="));
            calculateProcess.append(result);
            item = new QTableWidgetItem("=");
            item->setTextAlignment(Qt::AlignJustify);
            tableWidget->setItem(row,0,item);
            item = new QTableWidgetItem(result);
            item->setTextAlignment(Qt::AlignRight);
            tableWidget->setItem(row++,1,item);
        }
        list.removeFirst();
        operand1.clear();
        operand2.clear();
    }
    lineEdit->setText(result);

    /*while(!calculateProcess.isEmpty())
    {
        qDebug()<<calculateProcess.first()<<endl;
        calculateProcess.removeFirst();
    }*/


}
void MainWindow::viewChange()
{

    if(buttonFlag==false)
    {
        tableWidget->clear();
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并且平分

        tableWidget->setFont(QFont("Helvetica [Cronyx]",11,-1,false));
        changeView->setText(trUtf8("栈中数据变化"));
        tableWidget->setRowCount(stackChange.length());
        int row=0;
        QStringList temp;
        while(!stackChange.isEmpty())
        {
            tableWidget->setItem(row,0,new QTableWidgetItem(stackChange.first()));
            temp.append(stackChange.first());
            stackChange.removeFirst();

            QTableWidgetItem *item = new QTableWidgetItem(stackChange.first());

            tableWidget->setItem(row,1,item);

            temp.append(stackChange.first());
            stackChange.removeFirst();
            row++;
        }
        stackChange=temp;
        buttonFlag=true;

    }
    else
    {
        tableWidget->clear();
        tableWidget->setColumnWidth(0,20);
        tableWidget->setColumnWidth(1,140);
        tableWidget->setFont(QFont("Helvetica [Cronyx]",22,-1,false));
        changeView->setText(trUtf8("计算过程"));
        tableWidget->setRowCount(calculateProcess.length());
        int row=0;
        QStringList temp;

        while(!calculateProcess.isEmpty())
        {
            tableWidget->setItem(row,0,new QTableWidgetItem(calculateProcess.first()));
            temp.append(calculateProcess.first());
            calculateProcess.removeFirst();

            QTableWidgetItem *item = new QTableWidgetItem(calculateProcess.first());
            item->setTextAlignment(Qt::AlignRight);
            tableWidget->setItem(row,1,item);
            temp.append(calculateProcess.first());
            calculateProcess.removeFirst();
            row++;
        }
        calculateProcess=temp;

        buttonFlag=false;
    }
}
