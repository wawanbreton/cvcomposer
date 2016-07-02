// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#include "nodestypesmanager.h"

#include <QDebug>

#include "processor/analyzer/discretefouriertransformprocessor.h"
#include "processor/data/kernelprocessor.h"
#include "processor/filter/addweightedprocessor.h"
#include "processor/filter/bilateralfilterprocessor.h"
#include "processor/filter/blurprocessor.h"
#include "processor/filter/converttoprocessor.h"
#include "processor/filter/customfilterprocessor.h"
#include "processor/filter/gaussianblurprocessor.h"
#include "processor/filter/medianblurprocessor.h"
#include "processor/filter/morphologytransformationprocessor.h"
#include "processor/filter/thresholdprocessor.h"
#include "processor/filter/sobelprocessor.h"
#include "processor/filter/laplacianprocessor.h"
#include "processor/geometry/subimageprocessor.h"
#include "processor/geometry/makeborderprocessor.h"
#include "processor/input/cameraprocessor.h"
#include "processor/input/imagefromfileprocessor.h"
#include "processor/shape/drawcircleprocessor.h"
#include "processor/shape/drawellipseprocessor.h"
#include "processor/shape/drawlineprocessor.h"
#include "processor/shape/drawrectangleprocessor.h"
#include "processor/shape/rectangleprocessor.h"
#include "processor/shape/drawtextprocessor.h"
#include "processor/viewer/dockableimageviewerprocessor.h"
#include "processor/viewer/imagepreviewerprocessor.h"


QList<QPair<QString, QStringList> > NodesTypesManager::getNodes()
{
    #warning Do this somewhere else
    qRegisterMetaType<BlurProcessor>();
    qRegisterMetaType<ImageFromFileProcessor>();
    qRegisterMetaType<ImagePreviewerProcessor>();
    qRegisterMetaType<DockableImageViewerProcessor>();
    qRegisterMetaType<GaussianBlurProcessor>();
    qRegisterMetaType<MedianBlurProcessor>();
    qRegisterMetaType<BilateralFilterProcessor>();
    qRegisterMetaType<MorphologyTransformationProcessor>();
    qRegisterMetaType<ThresholdProcessor>();
    qRegisterMetaType<CameraProcessor>();
    qRegisterMetaType<KernelProcessor>();
    qRegisterMetaType<CustomFilterProcessor>();
    qRegisterMetaType<DiscreteFourierTransformProcessor>();
    qRegisterMetaType<RectangleProcessor>();
    qRegisterMetaType<DrawRectangleProcessor>();
    qRegisterMetaType<SubImageProcessor>();
    qRegisterMetaType<MakeBorderProcessor>();
    qRegisterMetaType<DrawCircleProcessor>();
    qRegisterMetaType<DrawEllipseProcessor>();
    qRegisterMetaType<DrawLineProcessor>();
    qRegisterMetaType<AddWeightedProcessor>();
    qRegisterMetaType<ConvertToProcessor>();
    qRegisterMetaType<DrawTextProcessor>();
    qRegisterMetaType<SobelProcessor>();
    qRegisterMetaType<LaplacianProcessor>();

    QList<QPair<QString, QStringList> > nodes;

    QStringList inputs;
    inputs << "ImageFromFile" << "Camera";
    nodes << QPair<QString, QStringList>("Inputs", inputs);

    QStringList geometry;
    geometry << "SubImage" << "MakeBorder";
    nodes << QPair<QString, QStringList>("Geometry", geometry);

    QStringList filters;
    filters << "Blur" << "GaussianBlur" << "MedianBlur" << "BilateralFilter"
            << "MorphologyTransformation" << "Threshold" << "Sobel" << "Laplacian" << "CustomFilter"
            << "AddWeighted" << "ConvertTo";
    nodes << QPair<QString, QStringList>("Filters", filters);

    QStringList data;
    data << "Kernel";
    nodes << QPair<QString, QStringList>("Data", data);

    QStringList shapes;
    shapes << "Rectangle" << "DrawRectangle" << "DrawCircle" << "DrawEllipse" << "DrawLine"
           << "DrawText";
    nodes << QPair<QString, QStringList>("Shapes", shapes);

    QStringList analyzers;
    analyzers << "DiscreteFourierTransform";
    nodes << QPair<QString, QStringList>("Analyzers", analyzers);

    QStringList viewers;
    viewers << "ImagePreviewer" << "DockableImageViewer";
    nodes << QPair<QString, QStringList>("Viewers", viewers);

    return nodes;
}

QString NodesTypesManager::toUserReadableName(const QString &name)
{
    if(name == "DockableImageViewer")
    {
        return "Advanced image viewer";
    }
    else if(name == "ConvertTo")
    {
        return "Scale";
    }
    else
    {
        QString result;

        result += name.at(0); // Take first upper char, unmodified

        foreach(const QChar &aChar, name.mid(1))
        {
            if(aChar.isUpper())
            {
                result += QString(" ") + aChar.toLower();
            }
            else
            {
                result += aChar;
            }
        }

        return result;
    }
}
