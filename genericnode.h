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

#ifndef GENERICNODE_H
#define GENERICNODE_H

#include <QObject>

#include "plug.h"
#include "properties.h"

#include <opencv2/core/core.hpp>

class GenericNode : public QObject
{
    Q_OBJECT

    public:
        explicit GenericNode(const QString &name,
                             const QString &userReadableName,
                             QObject *parent = NULL);

        const QString &getName() const;

        const QString &getUserReadableName() const;

        quint8 getNbInputs() const;

        const QList<Plug *> &getInputs() const;

        bool hasInput(Plug *input) const;

        quint8 getNbOutputs() const;

        const QList<Plug *> &getOutputs() const;

        bool hasOutput(Plug *output) const;

        void signalProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        void signalProcessUnavailable();

        const Properties &getProperties() const;

        void setProperties(const Properties &properties);

    public slots:
        void setProperty(const QString &name, const QVariant &value);

    signals:
        void propertyChanged(const QString &name, const QVariant &value);

        void processDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        void processUnavailable();

    private:
        const QString _name;
        const QString _userReadableName;
        QList<Plug *> _inputs;
        QList<Plug *> _outputs;
        Properties _properties;
};

#endif // GENERICNODE_H
