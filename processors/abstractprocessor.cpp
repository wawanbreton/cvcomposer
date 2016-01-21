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
    _properties()
{

}

AbstractProcessor::~AbstractProcessor()
{

}

void AbstractProcessor::setProperties(const Properties &properties)
{
    _properties = properties;
}

QList<cv::Mat> AbstractProcessor::process(const QList<cv::Mat> &inputs)
{
    Q_ASSERT(inputs.count() == getNbInputs());
    QList<cv::Mat> outputs = processImpl(inputs);
    Q_ASSERT(outputs.count() == getNbOutputs());

    return outputs;
}

QVariant AbstractProcessor::getProperty(const QString &name) const
{
    Properties::const_iterator iterator = _properties.find(name);
    if(iterator != _properties.end())
    {
        return iterator.value();
    }
    else
    {
        qCritical() << "AbstractProcessor::getProperty" << "No property named" << name;
        return QVariant();
    }
}
