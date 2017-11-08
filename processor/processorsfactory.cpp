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

#include "processorsfactory.h"

#include <QDebug>

#include "processor/analyzer/discretefouriertransformprocessor.h"
#include "processor/analyzer/findcontoursprocessor.h"
#include "processor/data/kernelprocessor.h"
#include "processor/data/sublistprocessor.h"
#include "processor/data/countlistprocessor.h"
#include "processor/data/splitchannelsprocessor.h"
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
#include "processor/geometry/pyramidprocessor.h"
#include "processor/input/cameraprocessor.h"
#include "processor/input/imagefromfileprocessor.h"
#include "processor/input/imagesfromfolderprocessor.h"
#include "processor/math/countnonzeroprocessor.h"
#include "processor/math/averageprocessor.h"
#include "processor/math/operatorprocessor.h"
#include "processor/shape/circleprocessor.h"
#include "processor/shape/convexhullprocessor.h"
#include "processor/shape/rectangleprocessor.h"
#include "processor/shape/drawtextprocessor.h"
#include "processor/shape/houghcircleprocessor.h"
#include "processor/shape/drawshapeprocessor.h"
#include "processor/shape/lineprocessor.h"
#include "processor/shape/ellipseprocessor.h"
#include "processor/shape/boundingrectprocessor.h"
#include "processor/viewer/imageviewerprocessor.h"
#include "processor/viewer/dataviewerprocessor.h"


QList<QPair<QString, QStringList> > ProcessorsFactory::getProcessors()
{
    QList<QPair<QString, QStringList> > processors;

    QStringList inputs;
    inputs << "ImageFromFile" << "ImagesFromFolder" << "Camera";
    processors << QPair<QString, QStringList>("Inputs", inputs);

    QStringList geometry;
    geometry << "SubImage" << "MakeBorder" << "Pyramid";
    processors << QPair<QString, QStringList>("Geometry", geometry);

    QStringList filters;
    filters << "Blur" << "GaussianBlur" << "MedianBlur" << "BilateralFilter"
            << "MorphologyTransformation" << "Threshold" << "Sobel" << "Laplacian" << "CustomFilter"
            << "AddWeighted" << "ConvertTo";
    processors << QPair<QString, QStringList>("Filters", filters);

    QStringList data;
    data << "Kernel" << "SubList" << "CountList" << "SplitChannels";
    processors << QPair<QString, QStringList>("Data", data);

    QStringList math;
    math << "CountNonZero" << "Average" << "Operator";
    processors << QPair<QString, QStringList>("Math", math);

    QStringList shapes;
    shapes << "Rectangle" << "Line"
           << "Circle" << "Ellipse"
           << "HoughCircle" << "ConvexHull"
           << "DrawText" << "DrawShape"
           << "BoundingRect";
    processors << QPair<QString, QStringList>("Shapes", shapes);

    QStringList analyzers;
    analyzers << "DiscreteFourierTransform" << "FindContours";
    processors << QPair<QString, QStringList>("Analyzers", analyzers);

    QStringList viewers;
    viewers << "DataViewer" << "ImageViewer";
    processors << QPair<QString, QStringList>("Viewers", viewers);

    return processors;
}

QString ProcessorsFactory::toUserReadableName(const QString &name)
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

AbstractProcessor *ProcessorsFactory::createProcessor(const QString &rawProcessorName)
{
    if(rawProcessorName == "Blur")
    {
        return new BlurProcessor();
    }
    else if(rawProcessorName == "ImageFromFile")
    {
        return new ImageFromFileProcessor();
    }
    else if(rawProcessorName == "ImagesFromFolder")
    {
        return new ImagesFromFolderProcessor();
    }
    else if(rawProcessorName == "DataViewer")
    {
        return new DataViewerProcessor();
    }
    else if(rawProcessorName == "ImageViewer")
    {
        return new ImageViewerProcessor();
    }
    else if(rawProcessorName == "GaussianBlur")
    {
        return new GaussianBlurProcessor();
    }
    else if(rawProcessorName == "MedianBlur")
    {
        return new MedianBlurProcessor();
    }
    else if(rawProcessorName == "BilateralFilter")
    {
        return new BilateralFilterProcessor();
    }
    else if(rawProcessorName == "MorphologyTransformation")
    {
        return new MorphologyTransformationProcessor();
    }
    else if(rawProcessorName == "Threshold")
    {
        return new ThresholdProcessor();
    }
    else if(rawProcessorName == "Camera")
    {
        return new CameraProcessor();
    }
    else if(rawProcessorName == "Kernel")
    {
        return new KernelProcessor();
    }
    else if(rawProcessorName == "CustomFilter")
    {
        return new CustomFilterProcessor();
    }
    else if(rawProcessorName == "DiscreteFourierTransform")
    {
        return new DiscreteFourierTransformProcessor();
    }
    else if(rawProcessorName == "Rectangle")
    {
        return new RectangleProcessor();
    }
    else if(rawProcessorName == "SubImage")
    {
        return new SubImageProcessor();
    }
    else if(rawProcessorName == "MakeBorder")
    {
        return new MakeBorderProcessor();
    }
    else if(rawProcessorName == "Circle")
    {
        return new CircleProcessor();
    }
    else if(rawProcessorName == "HoughCircle")
    {
        return new HoughCircleProcessor();
    }
    else if(rawProcessorName == "AddWeighted")
    {
        return new AddWeightedProcessor();
    }
    else if(rawProcessorName == "ConvertTo")
    {
        return new ConvertToProcessor();
    }
    else if(rawProcessorName == "DrawText")
    {
        return new DrawTextProcessor();
    }
    else if(rawProcessorName == "Sobel")
    {
        return new SobelProcessor();
    }
    else if(rawProcessorName == "Laplacian")
    {
        return new LaplacianProcessor();
    }
    else if(rawProcessorName == "CountNonZero")
    {
        return new CountNonZeroProcessor();
    }
    else if(rawProcessorName == "SubList")
    {
        return new SubListProcessor();
    }
    else if(rawProcessorName == "Average")
    {
        return new AverageProcessor();
    }
    else if(rawProcessorName == "Operator")
    {
        return new OperatorProcessor();
    }
    else if(rawProcessorName == "FindContours")
    {
        return new FindContoursProcessor();
    }
    else if(rawProcessorName == "CountList")
    {
        return new CountListProcessor();
    }
    else if(rawProcessorName == "SplitChannels")
    {
        return new SplitChannelsProcessor();
    }
    else if(rawProcessorName == "DrawShape")
    {
        return new DrawShapeProcessor();
    }
    else if(rawProcessorName == "Line")
    {
        return new LineProcessor();
    }
    else if(rawProcessorName == "Ellipse")
    {
        return new EllipseProcessor();
    }
    else if(rawProcessorName == "ConvexHull")
    {
        return new ConvexHullProcessor();
    }
    else if(rawProcessorName == "BoundingRect")
    {
        return new BoundingRectProcessor();
    }
    else if(rawProcessorName == "Pyramid")
    {
        return new PyramidProcessor();
    }
    else
    {
        qCritical() << "Unknown processor type" << rawProcessorName;
        return NULL;
    }
}
