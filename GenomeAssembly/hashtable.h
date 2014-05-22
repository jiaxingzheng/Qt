#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <QString>
#include <qmath.h>
#include <QDebug>

#include "table.h"
class HashTable {
private:
    Table *table;


    unsigned int curr;

public:
    HashTable();
    ~HashTable();
    void locate(unsigned int FirPrefix);
    QString getFirPrefix();
    QString getSecPrefix();
    QString getPostfix();
    unsigned short getFreInRead();
    unsigned short getFreInContig();
    void setFreInContig();

    unsigned int hash(QString str);
    QString recover(unsigned int num);
    void addTable(QString str);

};

#endif // HASHTABLE_H
