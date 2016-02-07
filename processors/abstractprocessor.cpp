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

#include "abstractprocessor.h"

#include <QDebug>


AbstractProcessor::AbstractProcessor()
{
}

AbstractProcessor::~AbstractProcessor()
{
}

const QList<PlugDefinition> &AbstractProcessor::getInputs()
{
    return _inputs;
}

const QList<PlugDefinition> &AbstractProcessor::getOutputs()
{
    return _outputs;
}

Properties AbstractProcessor::process(const Properties &inputs)
{
    // Check that given inputs match the expected inputs
    QList<QString> inputNames = inputs.keys();
    qSort(inputNames);

    QList<QString> expectedInputNames;
    foreach(const PlugDefinition &plug, _inputs)
    {
        expectedInputNames << plug.name;
    }
    qSort(expectedInputNames);

    Q_ASSERT(inputNames == expectedInputNames);

    // Do the actual computing
    Properties outputs = processImpl(inputs);

    // Check that computed outputs match the expected ouputs
    QList<QString> outputNames = outputs.keys();
    qSort(outputNames);

    QList<QString> expectedOutputNames;
    foreach(const PlugDefinition &plug, _outputs)
    {
        expectedOutputNames << plug.name;
    }
    qSort(expectedOutputNames);

    Q_ASSERT(outputNames == expectedOutputNames);

    // Everything is fine, give the outputs
    return outputs;
}

void AbstractProcessor::addInput(const PlugDefinition &definition)
{
    _inputs << definition;
}

void AbstractProcessor::addInput(const QString &name,
                                 PlugType::Enum type,
                                 const QVariant &defaultValue,
                                 const Properties &widgetProperties)
{
    addInput(makePlug(name, type, defaultValue, widgetProperties));
}

void AbstractProcessor::addEnumerationInput(const QString &name,
                                            const QList<QPair<QString, QVariant> > &values,
                                            const QVariant &defaultValue)
{
    PlugDefinition plug;
    plug.name = name;
    plug.type = PlugType::Enumeration;
    plug.widgetProperties.insert("values", QVariant::fromValue(values));
    plug.defaultValue = defaultValue;

    addInput(plug);
}

void AbstractProcessor::addOutput(const PlugDefinition &definition)
{
    _outputs << definition;
}

void AbstractProcessor::addOutput(const QString &userReadableName, PlugType::Enum type)
{
    addOutput(makePlug(userReadableName, type));
}

PlugDefinition AbstractProcessor::makePlug(const QString &name,
                                           PlugType::Enum type,
                                           const QVariant &defaultValue,
                                           const Properties &widgetProperties)
{
    PlugDefinition plug;
    plug.name = name;
    plug.type = type;
    plug.defaultValue = defaultValue;
    plug.widgetProperties = widgetProperties;

    return plug;
}
