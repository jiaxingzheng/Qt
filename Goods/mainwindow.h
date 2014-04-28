#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"Goods.h"
#include"LList.h"
#include"LQueue.h"
#include <QWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QStringList>
#include<QHeaderView>
#include<QComboBox>
#include<QSpinBox>
#include<QLabel>
#include<QLineEdit>
#include<QDebug>
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include<QIODevice>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private:
    QVBoxLayout *mainlayout;
    QPushButton *inventory;
    QPushButton *shipping;
    QPushButton *delivered;
    QTableWidget *table;
    QHBoxLayout *status;

    QLabel *categoryLabel;
    QComboBox *category;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *amountLabel;
    QSpinBox *amount;
    QLabel *priceLabel;
    QSpinBox *priceBox;
    QPushButton *add;
    QHBoxLayout *inventoryLayout;


    QPushButton *ship;
    QPushButton *deliver;
    QPushButton *quit;
    QHBoxLayout *buttonLayout;


   LList<Goods> *llist;
   LList<Goods> *llist1;
   LList<Goods> *llist2;
   LList<Goods> *llist3;
   LList<Goods> *llist4;

   LQueue<Goods> *queue;
   LQueue<Goods> *deliveredQueue;

    void initUI();
    void initData();
    void showAll();

signals:

private slots:
    void addGoods();
    void changeCategory(int index);
    void shipGoods();
    void deliverGoods();
    void shippingGoods();
    void inventoryGoods();
    void deliveredGoods();
    void quitProgram();

};

#endif // MAINWINDOW_H
