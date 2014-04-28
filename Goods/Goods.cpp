#include "Goods.h"

Goods::Goods()
{
}
Goods::Goods(QString name,QString number,QString price)
{

    this->name=name;
    this->number=number;
    this->price=price;

}
void Goods::setname(QString name)
{
        this->name=name;
}
void Goods::setnumber(QString number)
{
    this->number=number;
}
void Goods::setprice(QString price)
{
    this->price=price;
}

QString Goods::getname()
{
    return name;
}
QString Goods::getnumber()
{
    return number;
}
QString Goods::getprice()
{
    return price;
}

