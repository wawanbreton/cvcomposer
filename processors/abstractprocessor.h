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

        quint8 getNbInputs() const;

        const QList<PlugDefinition> &getInputs();

        quint8 getNbOutputs() const;

        const QList<PlugDefinition> &getOutputs();

        Properties process(const Properties &inputs);

    protected:
        void addInput(const PlugDefinition &definition);

        void addInput(const QString &name,
                      PlugType::Enum type,
                      const QVariant &defaultValue = QVariant(),
                      const Properties &widgetProperties = Properties());

        void addEnumerationInput(const QString &name,
                                 const QList<QPair<QString, QVariant> > &values,
                                 const QVariant &defaultValue);

        void addOutput(const PlugDefinition &definition);

        void addOutput(const QString &userReadableName, PlugType::Enum type);

        virtual Properties processImpl(const Properties &inputs) = 0;

    private:
        PlugDefinition makePlug(const QString &name,
                                PlugType::Enum type,
                                const QVariant &defaultValue = QVariant(),
                                const Properties &widgetProperties = Properties());

    private:
        QList<PlugDefinition> _inputs;
        QList<PlugDefinition> _outputs;
};

#endif // ABSTRACTPROCESSOR_H
