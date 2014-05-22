#ifndef TABLE_H
#define TABLE_H
class Table {
public:
    unsigned int secondPrefix;
    unsigned short postfix;
    unsigned short freInRead;
    unsigned short freInContig;
    Table(){
        freInContig = 0;
        freInRead = 0;
    }
};

#endif // TABLE_H
