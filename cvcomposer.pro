#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T23:02:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvcomposer
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    composerwidget.cpp \
    filterslistwidget.cpp

HEADERS  += mainwidget.h \
    composerwidget.h \
    filterslistwidget.h

FORMS    += mainwidget.ui
