#include "hashtable.h"

HashTable::HashTable()
{
    table = new Table[1048576];
}
void HashTable::addTable(QString str)
{


    //qDebug()<<str<<endl;
    QString temp[3];
    temp[0]=str.mid(0,10);
    temp[1]=str.mid(10,10);
    temp[2]=str.mid(20);

    unsigned int sum[3];
    for(int i=0;i<3;i++)
        sum[i]=hash(temp[i]);

    if(table[sum[0]].freInRead==0)
    {
        table[sum[0]].secondPrefix=sum[1];
        table[sum[0]].postfix=sum[2];
        table[sum[0]].freInRead++;
    }
    else
        table[sum[0]].freInRead++;

    //qDebug()<<"prefix:"<<sum[0]<<"  freInRead:"<<table[sum[0]].freInRead<<endl;
    /*for(int i=0;i<3;i++)
    {
        qDebug()<<sum[i]<<endl;
    }*/


}
unsigned int HashTable::hash(QString str)
{
    //qDebug()<<str<<endl;
    if(str.contains('A'))
        str.replace("A","00");
    if(str.contains('C'))
        str.replace("C","11");
    if(str.contains('T'))
        str.replace("T","01");
    if(str.contains('G'))
        str.replace("G","10");

    //qDebug()<<str<<endl;
    unsigned int sum=0;
    for(int i=str.length()-1;i>=0;i--)
    {

        if(str.at(str.length()-i-1)=='1')
        {
            sum+=qPow(2,i);

        }

    }
    //qDebug()<<sum;

    return sum;

}
void HashTable::locate(unsigned int FirPrefix)
{
    curr = FirPrefix;
}
unsigned short HashTable::getFreInContig()
{
    return table[curr].freInContig;
}
unsigned short HashTable::getFreInRead()
{
    return table[curr].freInRead;
}
QString HashTable::getSecPrefix()
{
    QString temp = recover(table[curr].secondPrefix);
    //qDebug()<<temp<<endl;
    QString str;
    while(temp.length()!=20)
    {
        temp.append('0');
    }
    //qDebug()<<temp<<endl;
    for(int i=19;i>=0;i-=2)
    {
        if(temp[i]=='0'&&temp[i-1]=='0')
            str.append('A');
        else if(temp[i]=='1'&&temp[i-1]=='1')
            str.append('C');
        else if(temp[i]=='0'&&temp[i-1]=='1')
            str.append('T');
        else if(temp[i]=='1'&&temp[i-1]=='0')
            str.append('G');
    }
    //qDebug()<<str<<endl;
    return str;
}
QString HashTable::getPostfix()
{
    QString temp = recover(table[curr].postfix);
    //qDebug()<<temp<<endl;
    QString str;
    while(temp.length()!=8)
    {
        temp.append('0');
    }
    //qDebug()<<temp<<endl;
    for(int i=7;i>=0;i-=2)
    {
        if(temp[i]=='0'&&temp[i-1]=='0')
            str.append('A');
        else if(temp[i]=='1'&&temp[i-1]=='1')
            str.append('C');
        else if(temp[i]=='0'&&temp[i-1]=='1')
            str.append('T');
        else if(temp[i]=='1'&&temp[i-1]=='0')
            str.append('G');
    }
    //qDebug()<<str<<endl;
    return str;
}
QString HashTable::recover(unsigned int num)
{
    QString str;
    while(num!=0)
    {
        str+=QString::number(num%2);
        num=num/2;
    }

    return str;
    //qDebug()<<str<<endl;

}
void HashTable::setFreInContig()
{
    table[curr].freInContig=1;
}
QString HashTable::getFirPrefix()
{
    QString temp = recover(curr);
    //qDebug()<<temp<<endl;
    QString str;
    while(temp.length()!=20)
    {
        temp.append('0');
    }
    //qDebug()<<temp<<endl;
    for(int i=19;i>=0;i-=2)
    {
        if(temp[i]=='0'&&temp[i-1]=='0')
            str.append('A');
        else if(temp[i]=='1'&&temp[i-1]=='1')
            str.append('C');
        else if(temp[i]=='0'&&temp[i-1]=='1')
            str.append('T');
        else if(temp[i]=='1'&&temp[i-1]=='0')
            str.append('G');
    }
    //qDebug()<<str<<endl;
    return str;
}
