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

#pragma once

#include <QObject>

#include <QMutex>

#include <opencv2/core/core.hpp>

#include "global/properties.h"
#include "model/plugdefinition.h"

class AbstractProcessor : public QObject
{
    Q_OBJECT

    public:
        AbstractProcessor();

        const QList<PlugDefinition> &getInputs();

        const QList<PlugDefinition> &getOutputs();

        Properties process(const Properties &inputs);

        virtual bool getKeepProcessing() const;

    signals:
        void progress(qreal value);

    protected:
        void addInput(const PlugDefinition &definition);

        void addInput(const QString &name,
                      PlugType::PlugTypes types,
                      const QVariant &defaultValue = QVariant(),
                      const Properties &widgetProperties = Properties(),
                      ThreeStateBool::Enum labelVisible = ThreeStateBool::None,
                      ProcessorListType::Enum listSupport = ProcessorListType::None);

        void addInput(const QString &name,
                      PlugType::PlugTypes types,
                      ProcessorListType::Enum listSupport);

        void addEnumerationInput(const QString &name,
                                 const QList<QPair<QString, QVariant> > &values,
                                 const QVariant &defaultValue = QVariant());

        void addOutput(const PlugDefinition &definition);

        void addOutput(const QString &userReadableName,
                       PlugType::PlugTypes types,
                       ProcessorListType::Enum listSupport = ProcessorListType::None);

        void listProgress(const QList<QVariant> &list);

        virtual Properties processImpl(const Properties &inputs) = 0;

        QMutex &accessMutex() { return _mutex; }

    private:
        PlugDefinition makePlug(const QString &name,
                                PlugType::PlugTypes types,
                                const QVariant &defaultValue = QVariant(),
                                const Properties &widgetProperties = Properties(),
                                ThreeStateBool::Enum labelVisible = ThreeStateBool::None,
                                ProcessorListType::Enum listSupport = ProcessorListType::None);

    private:
        QList<PlugDefinition> _inputs;
        QList<PlugDefinition> _outputs;
        int _listProgress;
        QMutex _mutex;
};
