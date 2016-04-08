#-------------------------------------------------
#
# Project created by QtCreator 2015-11-17T03:09:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    student.cpp \
    team.cpp \
    class_list.cpp \
    ktreewidget.cpp \
    helper.cpp

HEADERS  += mainwindow.h \
    class_list.h \
    student.h \
    team.h \
    ktreewidget.h \
    helper.h

FORMS    += mainwindow.ui
