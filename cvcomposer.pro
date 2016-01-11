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
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui


SOURCES += main.cpp\
        mainwidget.cpp \
    composerwidget.cpp \
    nodestypesmanager.cpp \
    nodes/abstractnode.cpp \
    nodes/blurnode.cpp \
    composerscene.cpp \
    nodes/imagefilenode.cpp \
    nodesviews/plugitem.cpp \
    nodesviews/connectionitem.cpp \
    composermodel.cpp \
    composerexecutor.cpp \
    composerscheduler.cpp \
    nodes/gaussianblurnode.cpp \
    nodes/imageviewernode.cpp \
    interactivegraphicsview.cpp \
    nodesviews/genericnodeitem.cpp \
    nodesviews/imagepreviewitem.cpp \
    nodesviews/imagevieweritem.cpp \
    nodesviews/imageviewerwidget.cpp \
    nodeslistwidget.cpp

HEADERS  += mainwidget.h \
    composerwidget.h \
    nodestypesmanager.h \
    nodes/abstractnode.h \
    nodes/blurnode.h \
    composerscene.h \
    nodes/imagefilenode.h \
    nodesviews/plugitem.h \
    nodesviews/connectionitem.h \
    composermodel.h \
    nodesviews/customitems.h \
    composerexecutor.h \
    composerscheduler.h \
    connection.h \
    nodes/gaussianblurnode.h \
    nodes/imageviewernode.h \
    interactivegraphicsview.h \
    nodesviews/genericnodeitem.h \
    nodesviews/imagepreviewitem.h \
    nodesviews/imagevieweritem.h \
    nodesviews/imageviewerwidget.h \
    nodeslistwidget.h

FORMS    += \
    mainwidget.ui \
    nodesviews/imagedockwidget.ui
