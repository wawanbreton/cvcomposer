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
#include "processor/viewer/dockableimageviewerprocessor.h"
#include "processor/viewer/imagepreviewerprocessor.h"


QList<QTreeWidgetItem *> NodesTypesManager::getTreeItems()
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

    QList<QTreeWidgetItem *> result;

    QTreeWidgetItem *inputs = new QTreeWidgetItem(QStringList() << "Inputs");
    inputs->setFlags(inputs->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemFile = new QTreeWidgetItem(inputs, QStringList() << "Image from file");
    itemFile->setData(0, Qt::UserRole, "ImageFromFile");
    QTreeWidgetItem *itemCamera = new QTreeWidgetItem(inputs, QStringList() << "Camera");
    itemCamera->setData(0, Qt::UserRole, "Camera");
    result << inputs;

    QTreeWidgetItem *viewers = new QTreeWidgetItem(QStringList() << "Viewers");
    viewers->setFlags(viewers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemDockableViewer = new QTreeWidgetItem(viewers, QStringList() << "Dockable image viewer");
    itemDockableViewer->setData(0, Qt::UserRole, "DockableImageViewer");
    QTreeWidgetItem *itemPreviewer = new QTreeWidgetItem(viewers, QStringList() << "Image previewer");
    itemPreviewer->setData(0, Qt::UserRole, "ImagePreviewer");
    result << viewers;

    QTreeWidgetItem *filters = new QTreeWidgetItem(QStringList() << "Filters");
    filters->setFlags(viewers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemBlur = new QTreeWidgetItem(filters, QStringList() << "Blur");
    itemBlur->setData(0, Qt::UserRole, "Blur");
    QTreeWidgetItem *itemGaussianBlur = new QTreeWidgetItem(filters, QStringList() << "Gaussian blur");
    itemGaussianBlur->setData(0, Qt::UserRole, "GaussianBlur");
    QTreeWidgetItem *itemMedianBlur = new QTreeWidgetItem(filters, QStringList() << "Median blur");
    itemMedianBlur->setData(0, Qt::UserRole, "MedianBlur");
    QTreeWidgetItem *itemBilateralFilter = new QTreeWidgetItem(filters, QStringList() << "Bilateral filter");
    itemBilateralFilter->setData(0, Qt::UserRole, "BilateralFilter");
    QTreeWidgetItem *itemMorph = new QTreeWidgetItem(filters, QStringList() << "Morphology transformation");
    itemMorph->setData(0, Qt::UserRole, "MorphologyTransformation");
    QTreeWidgetItem *itemThres = new QTreeWidgetItem(filters, QStringList() << "Threshold");
    itemThres->setData(0, Qt::UserRole, "Threshold");
    QTreeWidgetItem *itemCustom = new QTreeWidgetItem(filters, QStringList() << "Custom filter");
    itemCustom->setData(0, Qt::UserRole, "CustomFilter");
    result << filters;

    QTreeWidgetItem *data = new QTreeWidgetItem(QStringList() << "Data");
    data->setFlags(viewers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemKernel = new QTreeWidgetItem(data, QStringList() << "Kernel");
    itemKernel->setData(0, Qt::UserRole, "Kernel");
    result << data;

    QTreeWidgetItem *analyzers = new QTreeWidgetItem(QStringList() << "Analyzers");
    analyzers->setFlags(viewers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemDft = new QTreeWidgetItem(analyzers, QStringList() << "Discrete Fourier Transform");
    itemDft->setData(0, Qt::UserRole, "DiscreteFourierTransform");
    result << analyzers;

    return result;
}
