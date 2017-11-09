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


AbstractProcessor::AbstractProcessor() :
    _inputs(),
    _outputs(),
    _listProgress(0)
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
    // Check that given inputs match the expected inputs and check whether an input supports list
    QList<QString> inputNames = inputs.keys();
    qSort(inputNames);

    Properties listCompliantImputs;
    QList<QString> expectedInputNames;
    QString simpleInputListPlug;
    foreach(const PlugDefinition &plug, _inputs)
    {
        expectedInputNames << plug.name;

        if(plug.listSupport == ProcessorListType::None)
        {
            if(inputs[plug.name].userType() == qMetaTypeId<QList<QVariant> >())
            {
                // Input expects a single element, give it the first element of the list
                listCompliantImputs[plug.name] = inputs[plug.name].value<QList<QVariant> >().value(0, QVariant());
            }
            else
            {
                listCompliantImputs[plug.name] = inputs[plug.name];
            }
        }
        else
        {
            if(plug.listSupport == ProcessorListType::Simple)
            {
                if(!simpleInputListPlug.isEmpty())
                {
                    qCritical() << "Multiple simple list support is not implemented yet";
                }
                simpleInputListPlug = plug.name;
            }

            if(inputs[plug.name].userType() != qMetaTypeId<QList<QVariant> >())
            {
                // Input expects a list, give it a list containing the single element
                listCompliantImputs[plug.name] = QList<QVariant>() << inputs[plug.name];
            }
            else
            {
                listCompliantImputs[plug.name] = inputs[plug.name];
            }
        }
    }

    qSort(expectedInputNames);

    Q_ASSERT(inputNames == expectedInputNames);

    Properties outputs;

    // Compute the expected outputs names
    QList<QString> expectedOutputNames;
    for(const PlugDefinition &plug : _outputs)
    {
        expectedOutputNames << plug.name;
    }
    qSort(expectedOutputNames);

    // Do the actual computing
    if(simpleInputListPlug.isEmpty())
    {
        // No list support, just give the inputs and retrieve the outputs
        outputs = processImpl(listCompliantImputs);
    }
    else
    {
        // Simple list support : iterate on each value of the given list
        Properties singleOutputs;
        QList<QVariant> simpleListValues = listCompliantImputs[simpleInputListPlug].value<QList<QVariant> >();
        if(simpleListValues.isEmpty())
        {
            // Fill the outputs with empty lists
            for(const QString &expectedOutputName : expectedOutputNames)
            {
                outputs[expectedOutputName] = QList<QVariant>();
            }
        }
        else
        {
            listProgress(simpleListValues);
            foreach(const QVariant &simpleListValue, simpleListValues)
            {
                // For each element, extract it and process the computation
                Properties singleInputs = listCompliantImputs;
                singleInputs[simpleInputListPlug] = simpleListValue;

                singleOutputs = processImpl(singleInputs);

                // Then add the computed output values to the complete list of outputs
                for(auto it = singleOutputs.begin() ; it != singleOutputs.end() ; ++it)
                {
                    QList<QVariant> outputValues = outputs[it.key()].value<QList<QVariant> >();
                    outputValues << it.value();
                    outputs[it.key()] = outputValues;
                }

                listProgress(simpleListValues);
            }
        }
    }

    // Check that computed outputs match the expected ouputs
    QList<QString> outputNames = outputs.keys();
    qSort(outputNames);

    Q_ASSERT(outputNames == expectedOutputNames);

    // Everything is fine, give the outputs
    return outputs;
}

bool AbstractProcessor::getRealTimeProcessing() const
{
    return false;
}

void AbstractProcessor::addInput(const PlugDefinition &definition)
{
    _inputs << definition;
}

void AbstractProcessor::addInput(const QString &name,
                                 PlugType::PlugTypes types,
                                 const QVariant &defaultValue,
                                 const Properties &widgetProperties,
                                 ThreeStateBool::Enum labelVisible,
                                 ProcessorListType::Enum listSupport)
{
    addInput(makePlug(name, types, defaultValue, widgetProperties, labelVisible, listSupport));
}

void AbstractProcessor::addInput(const QString &name,
                                 PlugType::PlugTypes types,
                                 ProcessorListType::Enum listSupport)
{
    addInput(name, types, QVariant(), Properties(), ThreeStateBool::None, listSupport);
}

void AbstractProcessor::addEnumerationInput(const QString &name,
                                            const QList<QPair<QString, QVariant> > &values,
                                            const QVariant &defaultValue)
{
    Properties widgetProperties;
    widgetProperties.insert("values", QVariant::fromValue(values));

    addInput(makePlug(name,
                      PlugType::Enumeration,
                      defaultValue.isNull() ? values.first().second : defaultValue,
                      widgetProperties));
}

void AbstractProcessor::addOutput(const PlugDefinition &definition)
{
    _outputs << definition;
}

void AbstractProcessor::addOutput(const QString &userReadableName,
                                  PlugType::PlugTypes types,
                                  ProcessorListType::Enum listSupport)
{
    addOutput(makePlug(userReadableName,
                       types,
                       QVariant(),
                       Properties(),
                       ThreeStateBool::None,
                       listSupport));
}

void AbstractProcessor::listProgress(const QList<QVariant> &list)
{
    if(list.size() > 1)
    {
        emit progress(qreal(_listProgress++) / list.size());
    }
}

PlugDefinition AbstractProcessor::makePlug(const QString &name,
                                           PlugType::PlugTypes types,
                                           const QVariant &defaultValue,
                                           const Properties &widgetProperties,
                                           ThreeStateBool::Enum labelVisible,
                                           ProcessorListType::Enum listSupport)
{
    PlugDefinition plug;
    plug.name = name;
    plug.types = types;
    plug.defaultValue = defaultValue;
    plug.widgetProperties = widgetProperties;
    plug.labelVisible = labelVisible;
    plug.listSupport = listSupport;

    return plug;
}
