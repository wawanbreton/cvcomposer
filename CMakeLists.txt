cmake_minimum_required(VERSION 3.16)
project(cvcomposer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Widgets Xml)

qt_standard_project_setup()

qt_add_executable(cvcomposer
    execution/composerexecutor.cpp execution/composerexecutor.h
    execution/composerscheduler.cpp execution/composerscheduler.h
    execution/executorsettings.h
    global/cvconstants.h
    global/cvutils.cpp global/cvutils.h
    global/parser.cpp global/parser.h
    global/properties.cpp global/properties.h
    global/threestatebool.h
    global/utils.h
    gui/advancedimageviewer.cpp gui/advancedimageviewer.h
    gui/command/commandid.h
    gui/command/createnodecommand.cpp gui/command/createnodecommand.h
    gui/command/editconnectioncommand.cpp gui/command/editconnectioncommand.h
    gui/command/editvaluecommand.cpp gui/command/editvaluecommand.h
    gui/command/movenodecommand.cpp gui/command/movenodecommand.h
    gui/command/removenodecommand.cpp gui/command/removenodecommand.h
    gui/composerscene.cpp gui/composerscene.h
    gui/composerwidget.cpp gui/composerwidget.h
    gui/connectionitem.cpp gui/connectionitem.h
    gui/customitems.h
    gui/editsettingsdialog.cpp gui/editsettingsdialog.h gui/editsettingsdialog.ui
    gui/errordisplaydialog.cpp gui/errordisplaydialog.h gui/errordisplaydialog.ui
    gui/genericnodeitem.cpp gui/genericnodeitem.h
    gui/genericnodewidget.cpp gui/genericnodewidget.h
    gui/interactivegraphicsview.cpp gui/interactivegraphicsview.h
    gui/mainwidget.cpp gui/mainwidget.h gui/mainwidget.ui
    gui/plugitem.cpp gui/plugitem.h
    gui/plugtypeshelpdialog.cpp gui/plugtypeshelpdialog.h gui/plugtypeshelpdialog.ui
    gui/processorsitemmodel.cpp gui/processorsitemmodel.h
    gui/processorsmodelfilter.cpp gui/processorsmodelfilter.h
    main.cpp
    model/circle.h
    model/composermodel.cpp model/composermodel.h
    model/connection.cpp model/connection.h
    model/contour.h
    model/ellipse.h
    model/line.h
    model/node.cpp model/node.h
    model/plug.cpp model/plug.h
    model/plugdefinition.h
    model/plugtype.cpp model/plugtype.h
    model/segment.h
    model/triangle.h
    plugwidget/abstractplugwidget.cpp plugwidget/abstractplugwidget.h
    plugwidget/booleanwidget.cpp plugwidget/booleanwidget.h
    plugwidget/colorwidget.cpp plugwidget/colorwidget.h
    plugwidget/dataviewerwidget.cpp plugwidget/dataviewerwidget.h
    plugwidget/doubleitemdelegate.cpp plugwidget/doubleitemdelegate.h
    plugwidget/doublewidget.cpp plugwidget/doublewidget.h
    plugwidget/enumerationwidget.cpp plugwidget/enumerationwidget.h
    plugwidget/imageviewerdockwidget.cpp plugwidget/imageviewerdockwidget.h plugwidget/imageviewerdockwidget.ui
    plugwidget/imageviewerwidget.cpp plugwidget/imageviewerwidget.h
    plugwidget/kerneldefinitionwidget.cpp plugwidget/kerneldefinitionwidget.h
    plugwidget/pathwidget.cpp plugwidget/pathwidget.h
    plugwidget/pointwidget.cpp plugwidget/pointwidget.h
    plugwidget/sizewidget.cpp plugwidget/sizewidget.h
    plugwidget/stringwidget.cpp plugwidget/stringwidget.h
    processor/abstractprocessor.cpp processor/abstractprocessor.h
    processor/analyzer/discretefouriertransformprocessor.cpp processor/analyzer/discretefouriertransformprocessor.h
    processor/analyzer/findcontoursprocessor.cpp processor/analyzer/findcontoursprocessor.h
    processor/analyzer/haarcascadeprocessor.cpp processor/analyzer/haarcascadeprocessor.h
    processor/analyzer/mixtureofgaussian2processor.cpp processor/analyzer/mixtureofgaussian2processor.h
    processor/data/applycolormapprocessor.cpp processor/data/applycolormapprocessor.h
    processor/data/countlistprocessor.cpp processor/data/countlistprocessor.h
    processor/data/kernelprocessor.cpp processor/data/kernelprocessor.h
    processor/data/splitchannelsprocessor.cpp processor/data/splitchannelsprocessor.h
    processor/data/sublistprocessor.cpp processor/data/sublistprocessor.h
    processor/filter/absolutedifferenceprocessor.cpp processor/filter/absolutedifferenceprocessor.h
    processor/filter/adaptativethresholdprocessor.cpp processor/filter/adaptativethresholdprocessor.h
    processor/filter/addweightedprocessor.cpp processor/filter/addweightedprocessor.h
    processor/filter/applyroiprocessor.cpp processor/filter/applyroiprocessor.h
    processor/filter/bilateralfilterprocessor.cpp processor/filter/bilateralfilterprocessor.h
    processor/filter/blurprocessor.cpp processor/filter/blurprocessor.h
    processor/filter/cannyprocessor.cpp processor/filter/cannyprocessor.h
    processor/filter/convertcolorprocessor.cpp processor/filter/convertcolorprocessor.h
    processor/filter/converttoprocessor.cpp processor/filter/converttoprocessor.h
    processor/filter/customfilterprocessor.cpp processor/filter/customfilterprocessor.h
    processor/filter/floodfillprocessor.cpp processor/filter/floodfillprocessor.h
    processor/filter/gaussianblurprocessor.cpp processor/filter/gaussianblurprocessor.h
    processor/filter/inrangeprocessor.cpp processor/filter/inrangeprocessor.h
    processor/filter/laplacianprocessor.cpp processor/filter/laplacianprocessor.h
    processor/filter/medianblurprocessor.cpp processor/filter/medianblurprocessor.h
    processor/filter/morphologytransformationprocessor.cpp processor/filter/morphologytransformationprocessor.h
    processor/filter/sobelprocessor.cpp processor/filter/sobelprocessor.h
    processor/filter/thresholdprocessor.cpp processor/filter/thresholdprocessor.h
    processor/geometry/makeborderprocessor.cpp processor/geometry/makeborderprocessor.h
    processor/geometry/pyramidprocessor.cpp processor/geometry/pyramidprocessor.h
    processor/geometry/subimageprocessor.cpp processor/geometry/subimageprocessor.h
    processor/helpmessage.h
    processor/helpmessagetype.h
    processor/input/cameraprocessor.cpp processor/input/cameraprocessor.h
    processor/input/imagefromfileprocessor.cpp processor/input/imagefromfileprocessor.h
    processor/input/imagesfromfolderprocessor.cpp processor/input/imagesfromfolderprocessor.h
    processor/math/averageprocessor.cpp processor/math/averageprocessor.h
    processor/math/countnonzeroprocessor.cpp processor/math/countnonzeroprocessor.h
    processor/math/meanprocessor.cpp processor/math/meanprocessor.h
    processor/math/operatorprocessor.cpp processor/math/operatorprocessor.h
    processor/processorlisttype.h
    processor/processorsfactory.cpp processor/processorsfactory.h
    processor/shape/boundingrectprocessor.cpp processor/shape/boundingrectprocessor.h
    processor/shape/circleprocessor.cpp processor/shape/circleprocessor.h
    processor/shape/convexhullprocessor.cpp processor/shape/convexhullprocessor.h
    processor/shape/drawshapeprocessor.cpp processor/shape/drawshapeprocessor.h
    processor/shape/drawtextprocessor.cpp processor/shape/drawtextprocessor.h
    processor/shape/ellipseprocessor.cpp processor/shape/ellipseprocessor.h
    processor/shape/fitellipseprocessor.cpp processor/shape/fitellipseprocessor.h
    processor/shape/fitlineprocessor.cpp processor/shape/fitlineprocessor.h
    processor/shape/houghcircleprocessor.cpp processor/shape/houghcircleprocessor.h
    processor/shape/houghlineprobaprocessor.cpp processor/shape/houghlineprobaprocessor.h
    processor/shape/houghlineprocessor.cpp processor/shape/houghlineprocessor.h
    processor/shape/lineprocessor.cpp processor/shape/lineprocessor.h
    processor/shape/minenclosingtriangleprocessor.cpp processor/shape/minenclosingtriangleprocessor.h
    processor/shape/rectangleprocessor.cpp processor/shape/rectangleprocessor.h
    processor/viewer/dataviewerprocessor.cpp processor/viewer/dataviewerprocessor.h
    processor/viewer/imageviewerprocessor.cpp processor/viewer/imageviewerprocessor.h
)

target_link_libraries(cvcomposer PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Xml
)

target_compile_options(cvcomposer PRIVATE
    -Werror=return-type
    -Werror=shadow
    -Wno-deprecated-enum-enum-conversion
    -Wsuggest-override
    -std=c++2a
)

# Resources:
set(resources_resource_files
    "resources/Ubuntu-R.ttf"
    "resources/book.svg"
    "resources/brackets.svg"
    "resources/class.svg"
    "resources/credits.html"
    "resources/licence.html"
)

qt_add_resources(cvcomposer "resources"
    PREFIX
        "/"
    FILES
        ${resources_resource_files}
)

if(WIN32)
    target_include_directories(cvcomposer PRIVATE
        C:/Program Files/opencv3.1.0/build-mingw64/include
    )

    target_compile_definitions(cvcomposer PRIVATE
        _GLIBCXX_USE_CXX11_ABI=0
    )

    target_link_libraries(cvcomposer PRIVATE
        # Remove: L"C:/Program Files/opencv3.1.0/build-mingw64/lib"
        opencv_core310
        opencv_highgui310
        opencv_imgcodecs310
        opencv_imgproc310
        opencv_objdetect310
        opencv_video310
        opencv_videoio310
    )
endif()

if(UNIX)
    target_include_directories(cvcomposer PRIVATE
        /usr/include/opencv4
    )

    target_link_libraries(cvcomposer PRIVATE
        opencv_core
        opencv_highgui
        opencv_imgcodecs
        opencv_imgproc
        opencv_objdetect
        opencv_video
        opencv_videoio
    )
endif()

install(TARGETS cvcomposer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET cvcomposer
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
