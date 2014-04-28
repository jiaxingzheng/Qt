#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

    initUI();
    initData();

}
void MainWindow::initUI()
{
    mainlayout = new QVBoxLayout(this);


    inventory = new QPushButton(tr("Inventory"));
    connect(inventory,SIGNAL(clicked()),this,SLOT(inventoryGoods()));

    shipping = new QPushButton(tr("Shipping"));
    connect(shipping,SIGNAL(clicked()),this,SLOT(shippingGoods()));

    delivered = new QPushButton(tr("Delivered"));
    connect(delivered,SIGNAL(clicked()),this,SLOT(deliveredGoods()));

    status = new QHBoxLayout;

    status->addWidget(inventory);
    status->addWidget(shipping);
    status->addWidget(delivered);


    mainlayout->addLayout(status);




    table = new QTableWidget(0,3);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置为选择整行
    QStringList header;
    header.append(tr("Name"));
    header.append(tr("Amount"));
    header.append(tr("Price"));
    table->setHorizontalHeaderLabels(header);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑表格
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并且平分
    table->setTextElideMode(Qt::ElideMiddle);
    mainlayout->addWidget(table);




    categoryLabel = new QLabel(tr("Category:"));
    category = new QComboBox;
    category->insertItem(0,"All");
    category->insertItem(1,"PC");
    category->insertItem(2,"TV");
    category->insertItem(3,"laptop");
    category->insertItem(4,"camera");
    category->insertItem(5,"telephone");
    connect(category,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCategory(int)));

    nameLabel = new QLabel(tr("Name:"));
    nameEdit = new QLineEdit;
    amountLabel = new QLabel(tr("Amount:"));
    amount = new QSpinBox;
    amount->setMinimumWidth(100);
    amount->setMinimum(1);
    amount->setMaximum(1000000000);
    priceLabel = new QLabel(tr("Price:"));
    priceBox = new QSpinBox;
    priceBox->setMinimumWidth(100);
    priceBox->setMaximum(1000000000);
    priceBox->setMinimum(1);

    add = new QPushButton(tr("Add"));
    connect(add,SIGNAL(clicked()),this,SLOT(addGoods()));


    inventoryLayout = new QHBoxLayout;

    inventoryLayout->addWidget(categoryLabel);
    inventoryLayout->addWidget(category);
    inventoryLayout->addWidget(nameLabel);
    inventoryLayout->addWidget(nameEdit);
    inventoryLayout->addWidget(amountLabel);
    inventoryLayout->addWidget(amount);
    inventoryLayout->addWidget(priceLabel);
    inventoryLayout->addWidget(priceBox);
    inventoryLayout->addWidget(add);

    mainlayout->addLayout(inventoryLayout);



    ship = new QPushButton(tr("Ship"));
    connect(ship,SIGNAL(clicked()),this,SLOT(shipGoods()));
    deliver = new QPushButton(tr("Deliver"));
    deliver->setEnabled(false);
    connect(deliver,SIGNAL(clicked()),this,SLOT(deliverGoods()));
    quit = new QPushButton(tr("Quit"));
    connect(quit,SIGNAL(clicked()),this,SLOT(quitProgram()));


    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(ship);
    buttonLayout->addWidget(deliver);
    buttonLayout->addWidget(quit);

    mainlayout->addLayout(buttonLayout);


}
void MainWindow::initData()
{
    llist = new LList<Goods>;
    llist1 = new LList<Goods>;
    llist2 = new LList<Goods>;
    llist3 = new LList<Goods>;
    llist4 = new LList<Goods>;

    int row = 0;
    table->setRowCount(row+1);
    for(int i=0;i<5;i++)
    {
        QString filename;
        filename+=QString::number(i);
        filename+=QString(".txt");
        qDebug()<<filename;
        QFile file(filename);
        if(file.exists())
            qDebug()<<"exists";
        if (file.open(QFile::ReadOnly)) {
            qDebug()<<"open";
            Goods goods;
            //qDebug()<<file.readLine(200);
            QString line = file.readLine(200);
            qDebug()<<line;
            if(line.startsWith("\r")||line.isEmpty())
                continue;
            QStringList list= line.simplified().split(",");
            for (int col = 0; col < list.length(); ++col){
                        table->setItem(row, col,new QTableWidgetItem(list.at(col)));


                }
                ++row;
            table->setRowCount(row+1);
            goods.setname(list.at(0));
            goods.setnumber(list.at(1));
            goods.setprice(list.at(2));
            switch (i) {
            case 0:llist->append(goods);
                break;
            case 1:llist1->append(goods);
                break;
            case 2:llist2->append(goods);
                break;
            case 3:llist3->append(goods);
                break;
            case 4:llist4->append(goods);
                break;
            default:
                break;
            }


            while (file.canReadLine()) {
                qDebug()<<"canRead";
                Goods goods;
                QString line = file.readLine(200);
                qDebug()<<line;
                QStringList list= line.simplified().split(",");
                for (int col = 0; col < list.length(); ++col){
                            table->setItem(row, col,new QTableWidgetItem(list.at(col)));


                    }
                    ++row;
                table->setRowCount(row+1);
                goods.setname(list.at(0));
                goods.setnumber(list.at(1));
                goods.setprice(list.at(2));
                switch (i) {
                case 0:llist->append(goods);
                    break;
                case 1:llist1->append(goods);
                    break;
                case 2:llist2->append(goods);
                    break;
                case 3:llist3->append(goods);
                    break;
                case 4:llist4->append(goods);
                    break;
                default:
                    break;
                }
            }
        }
        file.close();


    }
    table->setRowCount(row);
    qDebug()<<llist1->length();
    for(int k=0;k<llist1->length();k++)
    {
        qDebug()<<llist1->getValue().getname();
        llist1->next();
    }
    queue = new LQueue<Goods>;
    deliveredQueue = new LQueue<Goods>;
    QFile file("shipping.txt");
    while (file.open(QFile::ReadOnly)) {
        qDebug()<<"open";
        Goods goods;
        QString line = file.readLine(200);
        qDebug()<<line;
        if(line.startsWith("\r")||line.isEmpty())
            break;
        QStringList list= line.simplified().split(",");
        goods.setname(list.at(0));
        goods.setnumber(list.at(1));
        goods.setprice(list.at(2));
        queue->enqueue(goods);
        while (file.canReadLine()) {

            qDebug()<<"canRead";
            Goods goods;
            QString line = file.readLine(200);
            qDebug()<<line;
            QStringList list= line.simplified().split(",");
            goods.setname(list.at(0));
            goods.setnumber(list.at(1));
            goods.setprice(list.at(2));
            queue->enqueue(goods);
        }
        break;
    }
    file.close();
    file.setFileName("delivered.txt");
    while(file.open(QFile::ReadOnly)) {
        qDebug()<<"open";
        Goods goods;
        QString line = file.readLine(200);
        qDebug()<<line;
        if(line.startsWith("\r")||line.isEmpty())
            break;
        QStringList list= line.simplified().split(",");
        goods.setname(list.at(0));
        goods.setnumber(list.at(1));
        goods.setprice(list.at(2));
        deliveredQueue->enqueue(goods);
        while (file.canReadLine()) {

            qDebug()<<"canRead";
            Goods goods;
            QString line = file.readLine(200);
            qDebug()<<line;
            QStringList list= line.simplified().split(",");
            goods.setname(list.at(0));
            goods.setnumber(list.at(1));
            goods.setprice(list.at(2));
            deliveredQueue->enqueue(goods);
        }
        break;
    }
    file.close();


}


void MainWindow::addGoods()
{
    if(nameEdit->text().isEmpty())
    {
        QMessageBox::information(this,"Prompt","Please input the name of the goods!");
    }
    else if(category->currentIndex()==0)
    {
        QMessageBox::information(this,"Prompt","Please select the category of the goods! ");
    }
    else
    {
        switch (category->currentIndex()) {
        case 1:{
            llist->moveToStart();
            for(int j=0;j<llist->length();j++)
            {
                if(llist->getValue().getname()==nameEdit->text())
                {
                    if(llist->getValue().getprice()==QString::number(priceBox->value()))
                    {
                        llist->getValue().setnumber(QString::number(llist->getValue().getnumber().toInt()+amount->value()));
                        table->setItem(llist->currPos(),1,new QTableWidgetItem(llist->getValue().getnumber()));
                        return;
                    }
                }
                llist->next();
            }
        }
        break;
        case 2:{
            llist1->moveToStart();
            for(int j=0;j<llist1->length();j++)
            {
                if(llist1->getValue().getname()==nameEdit->text())
                {
                    if(llist1->getValue().getprice()==QString::number(priceBox->value()))
                    {
                        llist1->getValue().setnumber(QString::number(llist1->getValue().getnumber().toInt()+amount->value()));
                        table->setItem(llist1->currPos(),1,new QTableWidgetItem(llist1->getValue().getnumber()));
                        return;
                    }
                }
                llist1->next();
            }
        }
        break;
        case 3:{
            llist2->moveToStart();
            for(int j=0;j<llist2->length();j++)
            {
                if(llist2->getValue().getname()==nameEdit->text())
                {
                    if(llist2->getValue().getprice()==QString::number(priceBox->value()))
                    {
                        llist2->getValue().setnumber(QString::number(llist2->getValue().getnumber().toInt()+amount->value()));
                        table->setItem(llist2->currPos(),1,new QTableWidgetItem(llist2->getValue().getnumber()));
                        return;
                    }
                }
                llist2->next();
            }
        }
        break;
        case 4:{
            llist3->moveToStart();
            for(int j=0;j<llist3->length();j++)
            {
                if(llist3->getValue().getname()==nameEdit->text())
                {
                    if(llist3->getValue().getprice()==QString::number(priceBox->value()))
                    {
                        llist3->getValue().setnumber(QString::number(llist3->getValue().getnumber().toInt()+amount->value()));
                        table->setItem(llist3->currPos(),1,new QTableWidgetItem(llist3->getValue().getnumber()));
                        return;
                    }
                }
                llist3->next();
            }
        }
        break;
        case 5:{
            llist4->moveToStart();
            for(int j=0;j<llist4->length();j++)
            {
                if(llist4->getValue().getname()==nameEdit->text())
                {
                    if(llist4->getValue().getprice()==QString::number(priceBox->value()))
                    {
                        llist4->getValue().setnumber(QString::number(llist4->getValue().getnumber().toInt()+amount->value()));
                        table->setItem(llist4->currPos(),1,new QTableWidgetItem(llist4->getValue().getnumber()));
                        return;
                    }
                }
                llist4->next();
            }
        }
        break;
        }


        Goods goods(nameEdit->text(),QString::number(amount->value()),QString::number(priceBox->value()));
        switch (category->currentIndex()) {
        case 1:{

            llist->append(goods);

        }
            break;
        case 2:{
            llist1->append(goods);

        }
            break;
        case 3:{
            llist2->append(goods);

        }
            break;
        case 4:{
            llist3->append(goods);

        }
            break;
        case 5:{
            llist4->append(goods);

        }
            break;
        default:
            break;
        }
        table->setRowCount(table->rowCount()+1);
        table->setItem(table->rowCount()-1,0,new QTableWidgetItem(nameEdit->text()));
        table->setItem(table->rowCount()-1,1,new QTableWidgetItem(QString::number(amount->value())));
        table->setItem(table->rowCount()-1,2,new QTableWidgetItem(QString::number(priceBox->value())));



    }

}
void MainWindow::changeCategory(int index)
{
    switch (index) {
    case 1:{
        llist->moveToStart();
        table->setRowCount(llist->length());
        for(int i=0;i<llist->length();i++)
        {
            table->setItem(i,0,new QTableWidgetItem(llist->getValue().getname()));
            table->setItem(i,1,new QTableWidgetItem(llist->getValue().getnumber()));
            table->setItem(i,2,new QTableWidgetItem(llist->getValue().getprice()));
            llist->next();
        }
    }
        break;
    case 2:{
        llist1->moveToStart();
        table->setRowCount(llist1->length());
        for(int i=0;i<llist1->length();i++)
        {
            table->setItem(i,0,new QTableWidgetItem(llist1->getValue().getname()));
            table->setItem(i,1,new QTableWidgetItem(llist1->getValue().getnumber()));
            table->setItem(i,2,new QTableWidgetItem(llist1->getValue().getprice()));
            llist1->next();
        }
    }
        break;
    case 3:{
        llist2->moveToStart();
        table->setRowCount(llist2->length());
        for(int i=0;i<llist2->length();i++)
        {
            table->setItem(i,0,new QTableWidgetItem(llist2->getValue().getname()));
            table->setItem(i,1,new QTableWidgetItem(llist2->getValue().getnumber()));
            table->setItem(i,2,new QTableWidgetItem(llist2->getValue().getprice()));
            llist2->next();
        }
    }
        break;
    case 4:{
        llist3->moveToStart();
        table->setRowCount(llist3->length());
        for(int i=0;i<llist3->length();i++)
        {
            table->setItem(i,0,new QTableWidgetItem(llist3->getValue().getname()));
            table->setItem(i,1,new QTableWidgetItem(llist3->getValue().getnumber()));
            table->setItem(i,2,new QTableWidgetItem(llist3->getValue().getprice()));
            llist3->next();
        }
    }
        break;
    case 5:{
        llist4->moveToStart();
        table->setRowCount(llist4->length());
        for(int i=0;i<llist4->length();i++)
        {
            table->setItem(i,0,new QTableWidgetItem(llist4->getValue().getname()));
            table->setItem(i,1,new QTableWidgetItem(llist4->getValue().getnumber()));
            table->setItem(i,2,new QTableWidgetItem(llist4->getValue().getprice()));
            llist4->next();
        }
    }
        break;
    default:{
        showAll();
    }
        break;
    }

}
void MainWindow::shipGoods()
{
    if(table->rowCount()==0)
    {
        QMessageBox::information(this,"Error","There are no goods in the inventory!");
        return;
    }
    switch (category->currentIndex()) {
    case 1:{
        llist->moveToPos(table->currentRow());
        table->removeRow(table->currentRow());
        queue->enqueue(llist->remove());
    }
        break;
    case 2:{
        llist1->moveToPos(table->currentRow());
        table->removeRow(table->currentRow());
        queue->enqueue(llist1->remove());
    }
        break;
    case 3:{
        llist2->moveToPos(table->currentRow());
        table->removeRow(table->currentRow());
        queue->enqueue(llist2->remove());
    }
        break;
    case 4:{
        llist3->moveToPos(table->currentRow());
        table->removeRow(table->currentRow());
        queue->enqueue(llist3->remove());
    }
        break;
    case 5:{
        llist4->moveToPos(table->currentRow());
        table->removeRow(table->currentRow());
        queue->enqueue(llist4->remove());
    }
        break;
    default:{
        if(table->currentRow()<llist->length())
        {
            llist->moveToPos(table->currentRow());
            queue->enqueue(llist->remove());
        }
        else if(table->currentRow()<llist->length()+llist1->length())
        {
            llist1->moveToPos(table->currentRow()-llist->length());
            queue->enqueue(llist1->remove());
        }
        else if(table->currentRow()<llist->length()+llist1->length()+llist2->length())
        {
            llist2->moveToPos(table->currentRow()-llist->length()-llist1->length());
            queue->enqueue(llist2->remove());
        }
        else if(table->currentRow()<llist->length()+llist1->length()+llist2->length()+llist3->length())
        {
            llist3->moveToPos(table->currentRow()-llist->length()-llist1->length()-llist2->length());
            queue->enqueue(llist3->remove());
        }
        else if(table->currentRow()<llist->length()+llist1->length()+llist2->length()+llist3->length()+llist4->length())
        {
            llist4->moveToPos(table->currentRow()-llist->length()-llist1->length()-llist2->length()-llist3->length());
            queue->enqueue(llist4->remove());
        }
        table->removeRow(table->currentRow());

    }
        break;
    }
}
void MainWindow::shippingGoods()
{
    table->setRowCount(queue->length());
    for(int i=0;i<queue->length();i++)
    {
         Goods goods=queue->dequeue();
         table->setItem(i,0,new QTableWidgetItem(goods.getname()));
         table->setItem(i,1,new QTableWidgetItem(goods.getnumber()));
         table->setItem(i,2,new QTableWidgetItem(goods.getprice()));
         queue->enqueue(goods);
    }

    category->setEnabled(false);
    nameEdit->setEnabled(false);
    amount->setEnabled(false);
    priceBox->setEnabled(false);
    add->setEnabled(false);
    ship->setEnabled(false);
    deliver->setEnabled(true);


}
void MainWindow::deliverGoods()
{
    if(table->rowCount()==0)
    {
        QMessageBox::information(this,"Error","There are no goods in the shipping queue!");
        return;
    }
    deliveredQueue->enqueue(queue->dequeue());
    table->removeRow(0);
}

void MainWindow::inventoryGoods()
{
    ship->setEnabled(true);
    category->setEnabled(true);
    nameEdit->setEnabled(true);
    amount->setEnabled(true);
    priceBox->setEnabled(true);
    add->setEnabled(true);
    deliver->setEnabled(false);
    if(category->currentIndex()!=0)
        category->setCurrentIndex(0);
    showAll();
}
void MainWindow::deliveredGoods()
{
    category->setEnabled(false);
    nameEdit->setEnabled(false);
    amount->setEnabled(false);
    priceBox->setEnabled(false);
    add->setEnabled(false);
    deliver->setEnabled(false);
    ship->setEnabled(false);
    table->setRowCount(deliveredQueue->length());
    for(int i=0;i<deliveredQueue->length();i++)
    {
         Goods goods=deliveredQueue->dequeue();
         table->setItem(i,0,new QTableWidgetItem(goods.getname()));
         table->setItem(i,1,new QTableWidgetItem(goods.getnumber()));
         table->setItem(i,2,new QTableWidgetItem(goods.getprice()));
         deliveredQueue->enqueue(goods);
    }



}
void MainWindow::showAll()
{
    table->setRowCount(llist->length()+llist1->length()+llist2->length()+llist3->length()+llist4->length());
    llist->moveToStart();
    for(int i=0;i<llist->length();i++)
    {
        table->setItem(i,0,new QTableWidgetItem(llist->getValue().getname()));
        table->setItem(i,1,new QTableWidgetItem(llist->getValue().getnumber()));
        table->setItem(i,2,new QTableWidgetItem(llist->getValue().getprice()));
        llist->next();
    }
    llist1->moveToStart();
    for(int i=0;i<llist1->length();i++)
    {
        table->setItem(llist->length()+i,0,new QTableWidgetItem(llist1->getValue().getname()));
        table->setItem(llist->length()+i,1,new QTableWidgetItem(llist1->getValue().getnumber()));
        table->setItem(llist->length()+i,2,new QTableWidgetItem(llist1->getValue().getprice()));
        llist1->next();
    }
    llist2->moveToStart();
    for(int i=0;i<llist2->length();i++)
    {
        table->setItem(llist->length()+llist1->length()+i,0,new QTableWidgetItem(llist2->getValue().getname()));
        table->setItem(llist->length()+llist1->length()+i,1,new QTableWidgetItem(llist2->getValue().getnumber()));
        table->setItem(llist->length()+llist1->length()+i,2,new QTableWidgetItem(llist2->getValue().getprice()));
        llist2->next();
    }
    llist3->moveToStart();
    for(int i=0;i<llist3->length();i++)
    {
        table->setItem(llist->length()+llist1->length()+llist2->length()+i,0,new QTableWidgetItem(llist3->getValue().getname()));
        table->setItem(llist->length()+llist1->length()+llist2->length()+i,1,new QTableWidgetItem(llist3->getValue().getnumber()));
        table->setItem(llist->length()+llist1->length()+llist2->length()+i,2,new QTableWidgetItem(llist3->getValue().getprice()));
        llist3->next();
    }
    llist4->moveToStart();
    for(int i=0;i<llist4->length();i++)
    {
        table->setItem(llist->length()+llist1->length()+llist2->length()+llist3->length()+i,0,new QTableWidgetItem(llist4->getValue().getname()));
        table->setItem(llist->length()+llist1->length()+llist2->length()+llist3->length()+i,1,new QTableWidgetItem(llist4->getValue().getnumber()));
        table->setItem(llist->length()+llist1->length()+llist2->length()+llist3->length()+i,2,new QTableWidgetItem(llist4->getValue().getprice()));
        llist4->next();
    }

}
void MainWindow::quitProgram()
{
    for(int i=0;i<5;i++)
    {


        switch (i) {
        case 0:{

            QFile file("0.txt");
            if (!file.open(QFile::WriteOnly | QFile::Text)) {
                QMessageBox::warning(this, "warning",
                                     "Cannot write file");

            }


            QTextStream out(&file);
            llist->moveToStart();
            for(int j=0;j<llist->length();j++)
            {
                out<<llist->getValue().getname()<<','<<llist->getValue().getnumber()<<','<<llist->getValue().getprice()<<"\n";
                llist->next();
            }
            file.close();
        }
            break;
        case 1:{

            QFile file("1.txt");
            if (!file.open(QFile::WriteOnly | QFile::Text)) {
                QMessageBox::warning(this, "warning",
                                     "Cannot write file");

            }

            QTextStream out(&file);
            llist1->moveToStart();
            for(int j=0;j<llist1->length();j++)
            {
                out<<llist1->getValue().getname()<<','<<llist1->getValue().getnumber()<<','<<llist1->getValue().getprice()<<"\n";
                llist1->next();
            }
            file.close();
        }
            break;
        case 2:{

            QFile file("2.txt");
            if (!file.open(QFile::WriteOnly | QFile::Text)) {
                QMessageBox::warning(this, "warning",
                                     "Cannot write file");

            }

            QTextStream out(&file);
            llist2->moveToStart();
            for(int j=0;j<llist2->length();j++)
            {
                out<<llist2->getValue().getname()<<','<<llist2->getValue().getnumber()<<','<<llist2->getValue().getprice()<<"\n";
                llist2->next();
            }
            file.close();
        }
            break;
        case 3:{

           QFile file("3.txt");
            if (!file.open(QFile::WriteOnly | QFile::Text)) {
                QMessageBox::warning(this, "warning",
                                     "Cannot write file");

            }

            QTextStream out(&file);
            llist3->moveToStart();
            for(int j=0;j<llist3->length();j++)
            {
                out<<llist3->getValue().getname()<<','<<llist3->getValue().getnumber()<<','<<llist3->getValue().getprice()<<"\n";
                llist3->next();
            }
            file.close();
        }
            break;
        case 4:{

            QFile file("4.txt");
            if(file.exists())
                qDebug()<<"exist";
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::warning(this, "warning",
                                     "Cannot write file");
                qDebug()<<file.error();
                 qDebug()<<file.errorString();


            }

            QTextStream out(&file);
            llist4->moveToStart();
            for(int j=0;j<llist4->length();j++)
            {
                out<<llist4->getValue().getname()<<','<<llist4->getValue().getnumber()<<','<<llist4->getValue().getprice()<<"\n";
                llist4->next();
            }
            file.close();
        }
            break;
        default:
            break;
        }


    }
    QFile file("shipping.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "warning",
                             "Cannot write file");
        qDebug()<<file.error();
         qDebug()<<file.errorString();


    }

    QTextStream out(&file);
    for(int k=0;k<queue->length();k++)
    {
        Goods goods = queue->dequeue();
        out<<goods.getname()<<','<<goods.getnumber()<<','<<goods.getprice()<<"\n";
    }
    file.close();

    file.setFileName("delivered.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "warning",
                             "Cannot write file");
        qDebug()<<file.error();
         qDebug()<<file.errorString();


    }

    out.setDevice(&file);
    for(int k=0;k<deliveredQueue->length();k++)
    {
        Goods goods = deliveredQueue->dequeue();
        out<<goods.getname()<<','<<goods.getnumber()<<','<<goods.getprice()<<"\n";
    }
    file.close();

    exit(0);
}
