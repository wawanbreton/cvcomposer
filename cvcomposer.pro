#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T23:02:19
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvcomposer
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11

win32 {
    INCLUDEPATH += "C:\Program Files\opencv2.4.6\build\include"
    LIBS += -L"C:\Program Files\opencv2.4.6\build-mingw\lib"
    LIBS += -lopencv_core249 -lopencv_imgproc249 -lopencv_highgui249
}
else {
    INCLUDEPATH += /usr/include/opencv2/
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui
}


SOURCES += main.cpp\
        mainwidget.cpp \
    composerwidget.cpp \
    nodestypesmanager.cpp \
    composerscene.cpp \
    nodesviews/plugitem.cpp \
    nodesviews/connectionitem.cpp \
    composermodel.cpp \
    composerexecutor.cpp \
    composerscheduler.cpp \
    interactivegraphicsview.cpp \
    nodesviews/genericnodeitem.cpp \
    nodeslistwidget.cpp \
    connection.cpp \
    cvutils.cpp \
    plugwidgets/imageviewerdockwidget.cpp \
    genericnode.cpp \
    processors/abstractprocessor.cpp \
    processors/blurprocessor.cpp \
    processors/imagefromfileprocessor.cpp \
    processors/imagepreviewerprocessor.cpp \
    processors/dockableimageviewerprocessor.cpp \
    processors/gaussianblurprocessor.cpp \
    processors/medianblurprocessor.cpp \
    processors/bilateralfilterprocessor.cpp \
    nodesviews/genericnodewidget.cpp \
    plugtype.cpp \
    plug.cpp \
    plugwidgets/imagepathwidget.cpp \
    plugwidgets/abstractplugwidget.cpp \
    plugwidgets/imagepreviewwidget.cpp \
    plugwidgets/sizewidget.cpp \
    plugwidgets/pointwidget.cpp \
    plugwidgets/enumerationwidget.cpp \
    plugwidgets/doublewidget.cpp \
    plugwidgets/stringwidget.cpp \
    plugwidgets/dockableimageviewerwidget.cpp \
    plugwidgets/colorwidget.cpp \
    processors/morphologytransformationprocessor.cpp \
    processors/thresholdprocessor.cpp \
    processors/cameraprocessor.cpp \
    processors/kernelprocessor.cpp \
    plugwidgets/kerneldefinitionwidget.cpp \
    properties.cpp \
    plugwidgets/doubleitemdelegate.cpp

HEADERS  += mainwidget.h \
    composerwidget.h \
    nodestypesmanager.h \
    composerscene.h \
    nodesviews/plugitem.h \
    nodesviews/connectionitem.h \
    composermodel.h \
    nodesviews/customitems.h \
    composerexecutor.h \
    composerscheduler.h \
    connection.h \
    interactivegraphicsview.h \
    nodesviews/genericnodeitem.h \
    nodeslistwidget.h \
    plug.h \
    cvutils.h \
    plugwidgets/imageviewerdockwidget.h \
    genericnode.h \
    processors/abstractprocessor.h \
    processors/blurprocessor.h \
    processors/imagefromfileprocessor.h \
    properties.h \
    processors/imagepreviewerprocessor.h \
    processors/dockableimageviewerprocessor.h \
    processors/gaussianblurprocessor.h \
    processors/medianblurprocessor.h \
    processors/bilateralfilterprocessor.h \
    plugdefinition.h \
    plugtype.h \
    nodesviews/genericnodewidget.h \
    plugwidgets/imagepathwidget.h \
    plugwidgets/abstractplugwidget.h \
    plugwidgets/imagepreviewwidget.h \
    plugwidgets/sizewidget.h \
    plugwidgets/pointwidget.h \
    plugwidgets/enumerationwidget.h \
    plugwidgets/doublewidget.h \
    plugwidgets/stringwidget.h \
    plugwidgets/dockableimageviewerwidget.h \
    plugwidgets/colorwidget.h \
    processors/morphologytransformationprocessor.h \
    processors/thresholdprocessor.h \
    processors/cameraprocessor.h \
    processors/kernelprocessor.h \
    plugwidgets/kerneldefinitionwidget.h \
    cvconstants.h \
    plugwidgets/doubleitemdelegate.h

FORMS    += \
    mainwidget.ui \
    plugwidgets/imagedockwidget.ui
