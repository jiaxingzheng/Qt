
#include<QString>

class Goods{
private:
    QString number;
    QString price;
    QString name;

public:
    Goods();
    Goods(QString name, QString number, QString price);
    void setname(QString name);
    void setnumber(QString number);
    void setprice(QString price);

    QString getname();
    QString getnumber();
    QString getprice();
};
