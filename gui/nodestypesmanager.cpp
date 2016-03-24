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
#include "processor/filter/bilateralfilterprocessor.h"
#include "processor/filter/blurprocessor.h"
#include "processor/filter/customfilterprocessor.h"
#include "processor/filter/gaussianblurprocessor.h"
#include "processor/filter/medianblurprocessor.h"
#include "processor/filter/morphologytransformationprocessor.h"
#include "processor/filter/thresholdprocessor.h"
#include "processor/input/cameraprocessor.h"
#include "processor/input/imagefromfileprocessor.h"
#include "processor/shape/rectangleprocessor.h"
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

    QList<QPair<QString, QStringList> > nodes;

    QStringList inputs;
    inputs << "ImageFromFile" << "Camera";
    nodes << QPair<QString, QStringList>("Inputs", inputs);

    QStringList filters;
    filters << "Blur" << "GaussianBlur" << "MedianBlur" << "BilateralFilter"
            << "MorphologyTransformation" << "Threshold" << "CustomFilter";
    nodes << QPair<QString, QStringList>("Filters", filters);

    QStringList data;
    data << "Kernel";
    nodes << QPair<QString, QStringList>("Data", data);

    QStringList shapes;
    shapes << "Rectangle";
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
    if(name == "ImageFromFile")
    {
        return "Image from file";
    }
    else if(name == "Camera")
    {
        return "Camera";
    }
    else if(name == "DockableImageViewer")
    {
        return "Advanced image viewer";
    }
    else if(name == "ImagePreviewer")
    {
        return "Image previewer";
    }
    else if(name == "Blur")
    {
        return "Blur";
    }
    else if(name == "GaussianBlur")
    {
        return "Gaussian blur";
    }
    else if(name == "MedianBlur")
    {
        return "Median blur";
    }
    else if(name == "BilateralFilter")
    {
        return "Bilateral filter";
    }
    else if(name == "MorphologyTransformation")
    {
        return "Morphology transformation";
    }
    else if(name == "Threshold")
    {
        return "Threshold";
    }
    else if(name == "CustomFilter")
    {
        return "Custom filter";
    }
    else if(name == "Kernel")
    {
        return "Kernel";
    }
    else if(name == "DiscreteFourierTransform")
    {
        return "Discrete Fourier Transform";
    }
    else if(name == "Rectangle")
    {
        return "Rectangle";
    }

    qCritical() << "No user-readable name defined for" << name;
    return "";
}
