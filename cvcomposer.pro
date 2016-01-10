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
    filterslistwidget.cpp \
    nodestypesmanager.cpp \
    nodes/abstractnode.cpp \
    nodes/blurnode.cpp \
    composerscene.cpp \
    nodesviews/abstractnodeview.cpp \
    nodes/imagefilenode.cpp \
    nodesviews/imagepreview.cpp \
    nodesviews/plugitem.cpp \
    nodesviews/connectionitem.cpp \
    composermodel.cpp \
    composerexecutor.cpp \
    composerscheduler.cpp \
    nodes/gaussianblurnode.cpp \
    nodesviews/imagedockviewer.cpp \
    nodes/imageviewernode.cpp \
    nodesviews/imagedockwidget.cpp

HEADERS  += mainwidget.h \
    composerwidget.h \
    filterslistwidget.h \
    nodestypesmanager.h \
    nodes/abstractnode.h \
    nodes/blurnode.h \
    composerscene.h \
    nodesviews/abstractnodeview.h \
    nodes/imagefilenode.h \
    nodesviews/imagepreview.h \
    nodesviews/plugitem.h \
    nodesviews/connectionitem.h \
    composermodel.h \
    nodesviews/customitems.h \
    composerexecutor.h \
    composerscheduler.h \
    connection.h \
    nodes/gaussianblurnode.h \
    nodesviews/imagedockviewer.h \
    nodes/imageviewernode.h \
    nodesviews/imagedockwidget.h

FORMS    += \
    mainwidget.ui \
    nodesviews/imagedockwidget.ui
