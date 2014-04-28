QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Goods
TEMPLATE = app

HEADERS += \
    mainwindow.h \
    Queue.h \
    LQueue.h \
    LList.h \
    List.h \
    Link.h \
    Goods.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    Goods.cpp

RESOURCES +=

RC_FILE = Goods.rc 

