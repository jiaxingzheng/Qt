#-------------------------------------------------
#
# Project created by QtCreator 2014-05-06T13:06:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncodeDecodeSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    HuffNode.h \
    IntlNode.h \
    LeafNode.h \
    HuffTree.h \
    heap.h
