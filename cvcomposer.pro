#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T23:02:19
#
#-------------------------------------------------

QT += core gui xml widgets

TARGET = cvcomposer
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11

win32 {
    DEFINES += "_GLIBCXX_USE_CXX11_ABI=0"
    INCLUDEPATH += "C:/Program Files/opencv3.1.0/build-mingw64/include"
    LIBS += -L"C:/Program Files/opencv3.1.0/build-mingw64/lib"
    LIBS += -lopencv_core310 -lopencv_imgproc310 -lopencv_highgui310 -lopencv_imgcodecs310 -lopencv_videoio310 -lopencv_objdetect310  -lopencv_video310
}
else {
    INCLUDEPATH += /usr/include/opencv4
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lopencv_objdetect -lopencv_video
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
    model/connection.cpp \
    global/cvutils.cpp \
    plugwidget/imageviewerdockwidget.cpp \
    processor/abstractprocessor.cpp \
    processor/data/applycolormapprocessor.cpp \
    processor/filter/blurprocessor.cpp \
    processor/filter/floodfillprocessor.cpp \
    processor/input/imagefromfileprocessor.cpp \
    processor/filter/gaussianblurprocessor.cpp \
    processor/filter/medianblurprocessor.cpp \
    processor/filter/bilateralfilterprocessor.cpp \
    gui/genericnodewidget.cpp \
    model/plugtype.cpp \
    model/plug.cpp \
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
    processor/geometry/subimageprocessor.cpp \
    processor/geometry/makeborderprocessor.cpp \
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
    processor/shape/circleprocessor.cpp \
    processor/shape/houghcircleprocessor.cpp \
    processor/data/sublistprocessor.cpp \
    processor/input/imagesfromfolderprocessor.cpp \
    processor/math/averageprocessor.cpp \
    processor/math/operatorprocessor.cpp \
    processor/analyzer/findcontoursprocessor.cpp \
    processor/data/countlistprocessor.cpp \
    processor/data/splitchannelsprocessor.cpp \
    plugwidget/pathwidget.cpp \
    processor/shape/drawshapeprocessor.cpp \
    processor/shape/lineprocessor.cpp \
    processor/shape/ellipseprocessor.cpp \
    gui/errordisplaydialog.cpp \
    gui/advancedimageviewer.cpp \
    processor/shape/convexhullprocessor.cpp \
    processor/shape/boundingrectprocessor.cpp \
    processor/geometry/pyramidprocessor.cpp \
    processor/filter/cannyprocessor.cpp \
    gui/processorsitemmodel.cpp \
    gui/processorsmodelfilter.cpp \
    processor/shape/houghlineprocessor.cpp \
    processor/shape/houghlineprobaprocessor.cpp \
    processor/analyzer/haarcascadeprocessor.cpp \
    processor/analyzer/mixtureofgaussian2processor.cpp \
    gui/plugtypeshelpdialog.cpp \
    processor/filter/convertcolorprocessor.cpp \
    processor/filter/applyroiprocessor.cpp

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
    model/plug.h \
    global/cvutils.h \
    plugwidget/imageviewerdockwidget.h \
    processor/abstractprocessor.h \
    processor/data/applycolormapprocessor.h \
    processor/filter/blurprocessor.h \
    processor/filter/floodfillprocessor.h \
    processor/input/imagefromfileprocessor.h \
    global/properties.h \
    processor/filter/gaussianblurprocessor.h \
    processor/filter/medianblurprocessor.h \
    processor/filter/bilateralfilterprocessor.h \
    model/plugdefinition.h \
    model/plugtype.h \
    gui/genericnodewidget.h \
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
    processor/geometry/subimageprocessor.h \
    processor/geometry/makeborderprocessor.h \
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
    model/circle.h \
    processor/shape/circleprocessor.h \
    processor/shape/houghcircleprocessor.h \
    global/utils.h \
    processor/data/sublistprocessor.h \
    processor/input/imagesfromfolderprocessor.h \
    processor/math/averageprocessor.h \
    processor/math/operatorprocessor.h \
    processor/analyzer/findcontoursprocessor.h \
    model/contour.h \
    processor/data/countlistprocessor.h \
    processor/data/splitchannelsprocessor.h \
    plugwidget/pathwidget.h \
    processor/shape/drawshapeprocessor.h \
    model/line.h \
    processor/shape/lineprocessor.h \
    processor/shape/ellipseprocessor.h \
    model/ellipse.h \
    processor/processorlisttype.h \
    gui/errordisplaydialog.h \
    gui/advancedimageviewer.h \
    processor/shape/convexhullprocessor.h \
    processor/shape/boundingrectprocessor.h \
    processor/geometry/pyramidprocessor.h \
    processor/filter/cannyprocessor.h \
    gui/processorsitemmodel.h \
    gui/processorsmodelfilter.h \
    processor/shape/houghlineprocessor.h \
    processor/shape/houghlineprobaprocessor.h \
    processor/analyzer/haarcascadeprocessor.h \
    processor/analyzer/mixtureofgaussian2processor.h \
    processor/helpmessagetype.h \
    processor/helpmessage.h \
    gui/plugtypeshelpdialog.h \
    processor/filter/convertcolorprocessor.h \
    processor/filter/applyroiprocessor.h

FORMS    += \
    gui/mainwidget.ui \
    gui/editsettingsdialog.ui \
    gui/errordisplaydialog.ui \
    plugwidget/imageviewerdockwidget.ui \
    gui/plugtypeshelpdialog.ui

RESOURCES += \
    resources.qrc
