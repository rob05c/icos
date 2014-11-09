#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T13:46:49
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = icos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp

HEADERS  += mainwindow.h \
    myglwidget.h

FORMS    += mainwindow.ui
