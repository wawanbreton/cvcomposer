#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T23:02:19
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvcomposer
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11

win32 {
    DEFINES += "_GLIBCXX_USE_CXX11_ABI=0"
    INCLUDEPATH += "C:\Program Files\opencv2.4.6\build\include"
    LIBS += -L"C:\Program Files\opencv2.4.6\build-mingw\lib"
    LIBS += -lopencv_core249 -lopencv_imgproc249 -lopencv_highgui249
}
else {
    INCLUDEPATH += /usr/include/opencv2/
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui
}


SOURCES += main.cpp\
        gui/mainwidget.cpp \
    gui/composerwidget.cpp \
    gui/composerscene.cpp \
    gui/plugitem.cpp \
    gui/connectionitem.cpp \
    model/composermodel.cpp \
    execution/composerexecutor.cpp \
    execution/composerscheduler.cpp \
    gui/interactivegraphicsview.cpp \
    gui/genericnodeitem.cpp \
    gui/nodeslistwidget.cpp \
    model/connection.cpp \
    global/cvutils.cpp \
    plugwidget/imageviewerdockwidget.cpp \
    processor/abstractprocessor.cpp \
    processor/filter/blurprocessor.cpp \
    processor/input/imagefromfileprocessor.cpp \
    processor/filter/gaussianblurprocessor.cpp \
    processor/filter/medianblurprocessor.cpp \
    processor/filter/bilateralfilterprocessor.cpp \
    gui/genericnodewidget.cpp \
    model/plugtype.cpp \
    model/plug.cpp \
    plugwidget/imagepathwidget.cpp \
    plugwidget/abstractplugwidget.cpp \
    plugwidget/sizewidget.cpp \
    plugwidget/pointwidget.cpp \
    plugwidget/enumerationwidget.cpp \
    plugwidget/doublewidget.cpp \
    plugwidget/stringwidget.cpp \
    plugwidget/colorwidget.cpp \
    processor/filter/morphologytransformationprocessor.cpp \
    processor/filter/thresholdprocessor.cpp \
    processor/input/cameraprocessor.cpp \
    processor/data/kernelprocessor.cpp \
    plugwidget/kerneldefinitionwidget.cpp \
    global/properties.cpp \
    plugwidget/doubleitemdelegate.cpp \
    processor/filter/customfilterprocessor.cpp \
    model/node.cpp \
    processor/analyzer/discretefouriertransformprocessor.cpp \
    processor/shape/rectangleprocessor.cpp \
    processor/shape/drawrectangleprocessor.cpp \
    processor/geometry/subimageprocessor.cpp \
    processor/geometry/makeborderprocessor.cpp \
    processor/shape/drawcircleprocessor.cpp \
    processor/shape/drawellipseprocessor.cpp \
    processor/shape/drawlineprocessor.cpp \
    processor/filter/addweightedprocessor.cpp \
    processor/filter/converttoprocessor.cpp \
    plugwidget/booleanwidget.cpp \
    processor/shape/drawtextprocessor.cpp \
    processor/filter/sobelprocessor.cpp \
    processor/filter/laplacianprocessor.cpp \
    gui/editsettingsdialog.cpp \
    global/parser.cpp \
    processor/processorsfactory.cpp \
    plugwidget/dataviewerwidget.cpp \
    processor/viewer/dataviewerprocessor.cpp \
    processor/viewer/imageviewerprocessor.cpp \
    plugwidget/imageviewerwidget.cpp \
    processor/math/countnonzeroprocessor.cpp \
    gui/boundedgraphicsproxywidget.cpp \
    processor/shape/circleprocessor.cpp \
    processor/shape/houghcircleprocessor.cpp \
    processor/data/sublistprocessor.cpp \
    processor/input/imagesfromfolderprocessor.cpp \
    plugwidget/folderpathwidget.cpp

HEADERS  += gui/mainwidget.h \
    gui/composerwidget.h \
    gui/composerscene.h \
    gui/plugitem.h \
    gui/connectionitem.h \
    model/composermodel.h \
    gui/customitems.h \
    execution/composerexecutor.h \
    execution/composerscheduler.h \
    model/connection.h \
    gui/interactivegraphicsview.h \
    gui/genericnodeitem.h \
    gui/nodeslistwidget.h \
    model/plug.h \
    global/cvutils.h \
    plugwidget/imageviewerdockwidget.h \
    processor/abstractprocessor.h \
    processor/filter/blurprocessor.h \
    processor/input/imagefromfileprocessor.h \
    global/properties.h \
    processor/filter/gaussianblurprocessor.h \
    processor/filter/medianblurprocessor.h \
    processor/filter/bilateralfilterprocessor.h \
    model/plugdefinition.h \
    model/plugtype.h \
    gui/genericnodewidget.h \
    plugwidget/imagepathwidget.h \
    plugwidget/abstractplugwidget.h \
    plugwidget/sizewidget.h \
    plugwidget/pointwidget.h \
    plugwidget/enumerationwidget.h \
    plugwidget/doublewidget.h \
    plugwidget/stringwidget.h \
    plugwidget/colorwidget.h \
    processor/filter/morphologytransformationprocessor.h \
    processor/filter/thresholdprocessor.h \
    processor/input/cameraprocessor.h \
    processor/data/kernelprocessor.h \
    plugwidget/kerneldefinitionwidget.h \
    global/cvconstants.h \
    plugwidget/doubleitemdelegate.h \
    processor/filter/customfilterprocessor.h \
    model/node.h \
    processor/analyzer/discretefouriertransformprocessor.h \
    processor/shape/rectangleprocessor.h \
    processor/shape/drawrectangleprocessor.h \
    processor/geometry/subimageprocessor.h \
    processor/geometry/makeborderprocessor.h \
    processor/shape/drawcircleprocessor.h \
    processor/shape/drawellipseprocessor.h \
    processor/shape/drawlineprocessor.h \
    processor/filter/addweightedprocessor.h \
    processor/filter/converttoprocessor.h \
    plugwidget/booleanwidget.h \
    processor/shape/drawtextprocessor.h \
    processor/filter/sobelprocessor.h \
    processor/filter/laplacianprocessor.h \
    gui/editsettingsdialog.h \
    execution/executorsettings.h \
    global/parser.h \
    global/threestatebool.h \
    processor/processorsfactory.h \
    plugwidget/dataviewerwidget.h \
    processor/viewer/dataviewerprocessor.h \
    processor/viewer/imageviewerprocessor.h \
    plugwidget/imageviewerwidget.h \
    processor/math/countnonzeroprocessor.h \
    gui/boundedgraphicsproxywidget.h \
    model/circle.h \
    processor/shape/circleprocessor.h \
    processor/shape/houghcircleprocessor.h \
    global/utils.h \
    processor/data/sublistprocessor.h \
    processor/input/imagesfromfolderprocessor.h \
    plugwidget/folderpathwidget.h

FORMS    += \
    gui/mainwidget.ui \
    plugwidget/imagedockwidget.ui \
    gui/editsettingsdialog.ui

RESOURCES += \
    resources.qrc
