#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T23:02:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvcomposer
TEMPLATE = app

INCLUDEPATH += /usr/include/opencv2/
LIBS += -lopencv_core -lopencv_imgproc


SOURCES += main.cpp\
        mainwidget.cpp \
    composerwidget.cpp \
    filterslistwidget.cpp \
    nodestypesmanager.cpp \
    nodes/abstractnode.cpp \
    nodes/blurnode.cpp \
    composerscene.cpp

HEADERS  += mainwidget.h \
    composerwidget.h \
    filterslistwidget.h \
    nodestypesmanager.h \
    abstractnode.h \
    nodes/abstractnode.h \
    nodes/blurnode.h \
    composerscene.h

FORMS    += mainwidget.ui
