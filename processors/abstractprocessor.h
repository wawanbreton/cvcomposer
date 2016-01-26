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

#ifndef ABSTRACTPROCESSOR_H
#define ABSTRACTPROCESSOR_H

#include <opencv2/core/core.hpp>

#include "properties.h"
#include "plugdefinition.h"

class AbstractProcessor
{
    public:
        AbstractProcessor();
        virtual ~AbstractProcessor();

        void setProperties(const Properties &properties);

        quint8 getNbInputs() const;

        const QList<PlugDefinition> &getInputs();

        quint8 getNbOutputs() const;

        const QList<PlugDefinition> &getOutputs();

        QList<cv::Mat> process(const QList<cv::Mat> &inputs);

    protected:
        void addInput(const PlugDefinition &definition);

        void addInput(const QString &userReadableName, PlugType::Enum type);

        void addOutput(const PlugDefinition &definition);

        void addOutput(const QString &userReadableName, PlugType::Enum type);

        virtual QList<cv::Mat> processImpl(const QList<cv::Mat> &inputs) = 0;

        QVariant getProperty(const QString &name) const;

    private:
        PlugDefinition makePlug(const QString &userReadableName, PlugType::Enum type);

    private:
        Properties _properties;
        QList<PlugDefinition> _inputs;
        QList<PlugDefinition> _outputs;
};

#endif // ABSTRACTPROCESSOR_H
