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

#include "mixtureofgaussian2processor.h"

#include <QDebug>

#include "global/cvutils.h"


MixtureOfGaussian2Processor::MixtureOfGaussian2Processor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Custom);

    // General properties
    Properties historyProperties;
    historyProperties.insert("decimals", 0);
    historyProperties.insert("minimum", 1);
    addInput("history", PlugType::Double, 500, historyProperties);

    Properties learningRateProperties;
    learningRateProperties.insert("decimals", 5);
    learningRateProperties.insert("minimum", -1);
    learningRateProperties.insert("maximum", 1);
    learningRateProperties.insert("singleStep", 0.1);
    addInput("learning rate", PlugType::Double, -1, learningRateProperties);

    Properties backgroundRatioProperties;
    backgroundRatioProperties.insert("decimals", 5);
    backgroundRatioProperties.insert("minimum", 0);
    backgroundRatioProperties.insert("maximum", 1);
    backgroundRatioProperties.insert("singleStep", 0.1);
    addInput("background ratio", PlugType::Double, 0.9, backgroundRatioProperties);

    Properties nMixturesProperties;
    nMixturesProperties.insert("decimals", 0);
    nMixturesProperties.insert("minimum", 1);
    addInput("number of mixtures", PlugType::Double, 5, nMixturesProperties);

    Properties crtProperties;
    crtProperties.insert("decimals", 5);
    crtProperties.insert("minimum", 0);
    crtProperties.insert("maximum", 1);
    crtProperties.insert("singleStep", 0.01);
    addInput("complexity reduction threshold", PlugType::Double, 0.05, crtProperties);

    // Shadow detection
    addInput("detect shadows", PlugType::Boolean, false);

    Properties shadowThresholdProperties;
    shadowThresholdProperties.insert("decimals", 3);
    shadowThresholdProperties.insert("minimum", 0);
    shadowThresholdProperties.insert("maximum", 1);
    shadowThresholdProperties.insert("singleStep", 0.1);
    addInput("shadow threshold", PlugType::Double, 0.5, shadowThresholdProperties);

    Properties shadowValueProperties;
    shadowValueProperties.insert("decimals", 0);
    shadowValueProperties.insert("minimum", 0);
    shadowValueProperties.insert("maximum", 255);
    addInput("shadow value", PlugType::Double, 127, shadowValueProperties);

    // Variance parameters
    Properties varianceProperties;
    varianceProperties.insert("decimals", 2);
    varianceProperties.insert("minimum", 0);
    addInput("initial variance", PlugType::Double, 15, varianceProperties);
    addInput("minimum variance", PlugType::Double, 4, varianceProperties);
    addInput("maximum variance", PlugType::Double, 75, varianceProperties);
    addInput("variance threshold", PlugType::Double, 16, varianceProperties);
    addInput("variance threshold gen.", PlugType::Double, 9, varianceProperties);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Custom);

    // Help
    addHelpMessage("BackgroundSubtractorMOG2",
                   CvUtils::makeUrl({"d7", "d7b", "classcv_1_1BackgroundSubtractorMOG2"}));
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"d1", "dc5", "tutorial_background_subtraction"}));
}

Properties MixtureOfGaussian2Processor::processImpl(const Properties &inputs)
{
    QList<QVariant> inputImages = inputs["input image"].value<QList<QVariant>>();
    QList<QVariant> outputImages;

    // Use a QMutexLocker in case of OpenCV exception
    QMutexLocker locker(&accessMutex());

    if(!_mog || inputImages.count() > 1)
    {
        _mog = cv::createBackgroundSubtractorMOG2();
    }

    _mog->setHistory(inputs["history"].toInt());
    _mog->setBackgroundRatio(inputs["background ratio"].toDouble());
    _mog->setNMixtures(inputs["number of mixtures"].toInt());
    _mog->setComplexityReductionThreshold(inputs["complexity reduction threshold"].toDouble());

    _mog->setDetectShadows(inputs["detect shadows"].toBool());
    _mog->setShadowThreshold(inputs["shadow threshold"].toDouble());
    _mog->setShadowValue(inputs["shadow value"].toInt());

    _mog->setVarInit(inputs["initial variance"].toDouble());
    _mog->setVarMin(inputs["minimum variance"].toDouble());
    _mog->setVarMax(inputs["maximum variance"].toDouble());
    _mog->setVarThreshold(inputs["variance threshold"].toDouble());
    _mog->setVarThresholdGen(inputs["variance threshold gen."].toDouble());

    for(QVariant &inputImage : inputImages)
    {
        cv::Mat image = inputImage.value<cv::Mat>();
        cv::Mat outputImage;
        _mog->apply(image, outputImage, inputs["learning rate"].toDouble());

        outputImages << QVariant::fromValue(outputImage);
    }

    locker.unlock();

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImages));
    return outputs;
}

